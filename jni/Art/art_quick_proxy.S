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

ENTRY exe_switch_entry
    push   {r1-r7, lr}
    blx    switch_entry
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
    mov    r7, lr
    str    r0, [sp]
    str    r1, [sp, #4]
    str    r2, [sp, #8]
    str    r3, [sp, #12]        @construct the args sp
    add    r2, sp, #4
    mov    r1, r9
    blx    box_args
    mov    r3, r0               @r3=box
    ldr    r2, [sp, #4]         @r2=this
    ldr    r1, [sp]             @r1=method
    mov    r0, #3
    push   {r1-r7)
    blx    switch_entry
    ldr    r6, [r0, #METHOD_QUICK_CODE_OFFSET_32]
    mov    ip, r6
    pop    {r1-r7)
    mov    lr, r7
    bx     ip
END art_quick_proxy
