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
    mov    r0, #0
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
