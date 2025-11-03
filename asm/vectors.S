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

