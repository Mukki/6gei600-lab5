# Modules Overview

A quick map from files to responsibilities, with the most important registers and functions you will touch.

## `src/main.c`
- Orchestrates initialization: `gpio_init()`, `interrupts_init()`, `timer_init()`
- Enters an infinite low‑power loop using `wfi` (wait for interrupt)

## `asm/start.S`
- Sets stack pointer, zeroes `.bss`
- Installs vector table to `VBAR_EL1`
- Enables IRQs and branches to `main`

## `asm/vectors.S`
- Vector table label `vectors`
- IRQ entry at EL1 (`irq_el1h`) saves minimal state, calls `irq_handler`, restores, `eret`

## `src/interrupts.c`
- Configures interrupt controller to enable System Timer Match 1
- `irq_handler()`:
  - Confirms pending source is System Timer Match 1
  - Clears timer match in `TIMER_CS`
  - Increments `tick_count`; toggles LED via `gpio_set_red()` every 100 ticks
  - Calls `timer_schedule_next()` to arm next interrupt

## `src/timer.c`
- Talks to System Timer registers at `0x3F003000`
- `timer_init()` arms first compare and clears pending flag
- `timer_schedule_next()` sets next compare = `CLO + tick_interval_us`
- `tick_interval_us` is 10,000 µs (100 Hz tick)

## `src/gpio.c`
- Configures GPIO function select registers to set pins as outputs
- `gpio_set_red(bool on)` turns the red LED on/off
- Constants assume Raspberry Pi 3 peripheral base `0x3F000000`

## `include/*.h`
- Public interfaces:
  - `gpio.h`: `gpio_init`, `gpio_set_red`
  - `timer.h`: `timer_init`, `timer_schedule_next`
  - `interrupts.h`: `interrupts_init`, `irq_handler`

## `linker.ld`
- Places code at load address `0x80000`
- Exposes `__bss_start`, `__bss_end`, and `_stack_top` used by `start.S`

## How to extend
- Add your own module `src/my_logic.c` with a header in `include/`
- Expose `void tick(void);` and call it from `irq_handler()` after clearing the timer and before scheduling the next tick
- Use GPIO helpers to manipulate LEDs or read pins

