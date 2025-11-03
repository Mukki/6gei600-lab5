# Lab5 Bare‑Metal Guide

Welcome! This guide explains how this Raspberry Pi 3B bare‑metal project boots, handles interrupts, and toggles an LED using the system timer. Use this as your starting point.

## What you will learn
- How boot code in assembly hands off to C (`asm/start.S`, `asm/vectors.S`)
- How the program initializes and sleeps waiting for interrupts (`src/main.c`)
- How the system timer generates periodic interrupts (`src/timer.c`)
- How the interrupt handler schedules the next tick and toggles an LED (`src/interrupts.c`)
- How GPIO is configured to drive an LED (`src/gpio.c`)
- How the image is linked and loaded (`linker.ld`)

## Recommended reading order
1. Boot and interrupt flow
2. Modules overview
3. Build and run
4. Memory layout
5. Troubleshooting

## Docs index
- Boot and interrupt flow: `docs/boot_interrupt_flow.md`
- Modules overview: `docs/modules.md`
- Build and run: `docs/build_and_run.md`
- Memory layout: `docs/memory_layout.md`
- Troubleshooting: `docs/troubleshooting.md`

If you get stuck, skim the troubleshooting page first.


