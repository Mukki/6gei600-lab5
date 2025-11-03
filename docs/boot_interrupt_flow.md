# Boot and Interrupt Flow

This project runs with no operating system (bare‑metal). The Raspberry Pi firmware loads `kernel8.img` into memory and jumps to `_start`.

## 1) Reset entry: `_start` (AArch64)
Key steps performed in `asm/start.S`:

```1:29:./Lab5/asm/start.S
.section .text.boot
.align 7
.globl _start
_start:
    ldr x0, =_stack_top
    mov sp, x0

    ldr x1, =__bss_start
    ldr x2, =__bss_end
1:
    cmp x1, x2
    b.hs 2f
    str xzr, [x1], #8
    b 1b
2:

    ldr x0, =vectors
    msr VBAR_EL1, x0
    isb

    msr DAIFClr, #2

    bl main

hang:
    wfe
    b hang
```

- Stack is set to `_stack_top` from `linker.ld`.
- `.bss` is zero‑initialized.
- Vector Base Address Register (`VBAR_EL1`) is set to our vector table symbol `vectors`.
- IRQs are enabled (`DAIFClr, #2`).
- C entry point `main()` is called.

## 2) Vector table and IRQ entry
The vector table is defined in `asm/vectors.S`. For IRQs at EL1, control flows to `irq_el1h` which saves registers, calls `irq_handler` in C, then returns with `eret`.

```1:25:./Lab5/asm/vectors.S
.section .text
.align 11
.globl vectors
.extern irq_handler
vectors:
    b .
    b .
    b .
    b .
    b .
    b irq_el1h
    b .
    b .

irq_el1h:
    sub sp, sp, #32
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    bl irq_handler
    ldp x2, x3, [sp, #16]
    ldp x0, x1, [sp, #0]
    add sp, sp, #32
    eret
```

## 3) C initialization and main loop
`main()` initializes peripherals and then waits for interrupts:

```8:16:./Lab5/src/main.c
int main(void) {
    gpio_init();
    interrupts_init();
    timer_init();

    for (;;) {
        __asm__ volatile ("wfi");
    }
}
```

- `gpio_init()` prepares the LED GPIO as output and turns it off.
- `interrupts_init()` enables the system timer match‑1 interrupt line in the interrupt controller.
- `timer_init()` arms the first timer compare for a future tick.
- The CPU executes `wfi` (wait‑for‑interrupt) in a loop to save power.

## 4) Timer IRQ handling path
When the system timer reaches the compare value, an interrupt is raised:

- Hardware vectors to `irq_el1h` → calls `irq_handler()`.
- In `irq_handler()` we:
  - Verify the pending source is system timer match‑1.
  - Acknowledge/clear the timer match in `TIMER_CS`.
  - Update our tick counter and toggle the LED every 100 ticks.
  - Schedule the next tick by writing a new compare value.

```26:44:./Lab5/src/interrupts.c
void irq_handler(void) {
    if (IRQ_PENDING1 & SYS_TIMER_MATCH1_IRQ) {
        if (TIMER_CS & CS_M1) {
            TIMER_CS = CS_M1;
            tick_count++;
            if (tick_count >= 100u) {
                tick_count = 0;
                led_state = !led_state;
                gpio_set_red(led_state);
            }
            timer_schedule_next();
        }
        return;
    }
}
```

And the tick scheduling helper:

```15:23:./Lab5/src/timer.c
void timer_init(void) {
    uint32_t now = TIMER_CLO;
    TIMER_C1 = now + tick_interval_us;
    TIMER_CS = CS_M1;
}

void timer_schedule_next(void) {
    TIMER_C1 = TIMER_CLO + tick_interval_us;
}
```

That’s the full loop: boot → set vectors → init → sleep → timer IRQ → handler → schedule next.
