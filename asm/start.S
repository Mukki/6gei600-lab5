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

