# Troubleshooting

Quick checks if the LED does not toggle as expected.

## Nothing happens at power on
- SD card missing Pi firmware files (`bootcode.bin`, `start.elf`, etc.)
- Wrong kernel name in `config.txt`
  - Must include:
    ```
    arm_64bit=1
    kernel=kernel8.img
    ```
- `kernel8.img` not on the boot partition
- Using wrong Pi model (addresses assume Raspberry Pi 3B)

## Builds but no LED activity
- Wiring mismatch: Confirm LED is on GPIO 17 (pin 11) with proper resistor and ground
- `gpio_init()` must run before interrupts; check call order in `main`
- Compare interval too long/short: `tick_interval_us` in `src/timer.c`
- Ensure interrupts are enabled in `interrupts_init()` and that `timer_init()` sets the first compare

## Interrupt fires only once
- In `irq_handler()`, the timer match flag must be cleared:
  - `TIMER_CS = CS_M1;` must execute before scheduling next compare

## Porting to a different Pi SoC
- Update `PERIPH_BASE` addresses in `gpio.c`, `timer.c`, `interrupts.c`
- On Pi 4, base is typically `0xFE000000` (not covered here)

## Serial debugging (optional)
- Add simple GPIO pin toggles in different places to see progress on a scope/LED
- Or implement a minimal UART to print messages (advanced)

