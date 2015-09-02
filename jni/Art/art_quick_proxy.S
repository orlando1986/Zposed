#include "asm_support.h"

.macro ENTRY name
    .thumb_func
    .type \name, #function
    .global \name
    .balign 16
\name:
    .cfi_startproc
    .fnstart
.endm

.macro END name
    .fnend
    .cfi_endproc
    .size \name, .-\name
.endm

.extern switch_entry
.extern box_args
.extern get_args_count

ENTRY get_object_addr
    mov    r0, r1
    bx     lr
END get_object_addr

ENTRY exe_switch_entry
    push   {r1-r7, lr}
    bl     switch_entry
    ldr    r7, [r0, #METHOD_QUICK_CODE_OFFSET_32]
    mov    ip, r7
    blx    ip
    pop    {r1-r7, pc}
END exe_switch_entry

ENTRY art_quick_proxy
    push   {r0-r7}
    mov    r7, lr
    mov    r0, #0
    bl     exe_switch_entry     @call getEntryTag
    mov    ip, r0
    mov    lr, r7
    cmp    r0, #0
    pop    {r0-r7}
    beq    PROXY
    bx     ip                   @call original method

PROXY:
    push   {r0, r1, r4-r7}              @sp-24
    ldr    r4, [sp, #24]
    ldr    r5, [sp, #(24+4)]
    ldr    r6, [sp, #(24+8)]
    ldr    r7, [sp, #(24+12)]
    push   {r4-r7}              @sp-40

    mov    r7, lr
    str    r0, [sp, #40]        @r0=method
    str    r1, [sp, #(40 + 4)]
    str    r2, [sp, #(40 + 8)]
    str    r3, [sp, #(40 + 12)]
    add    r2, sp, #(40 + 4)          @r2=args
    mov    r1, r9
    bl     box_args
    mov    lr, r7
    mov    r3, r0               @r3=box

    pop    {r4-r7}
    str    r4, [sp, #24]
    str    r5, [sp, #(24+4)]
    str    r6, [sp, #(24+8)]
    str    r7, [sp, #(24+12)]
    pop    {r0, r1, r4-r7}

    mov    r2, r1               @r2=this
    mov    r1, r0               @r1=method
    mov    r0, #3
    push   {r1-r7}
    mov    r7, lr
    bl     switch_entry
    mov    lr, r7
    ldr    r7, [r0, #METHOD_QUICK_CODE_OFFSET_32]
    mov    ip, r7
    pop    {r1-r7}
    bx     ip
END art_quick_proxy
