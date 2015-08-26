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

/*
 * switchEntry flag:
 * 0: get entry of the original or the proxy
 * 1: box arguments
 * 2: call proxy method
 */

.extern switchEntry
ENTRY art_quick_dispatcher
	push   {r0-r4}
	mov    r4, lr
	mov    r0, #0
	push   {r1-r7}
	blx    switchEntry
	pop    {r1-r7}
	mov    ip, r0
	mov    lr, r4
	pop    {r0-r4}
	bx     ip
END art_quick_dispatcher

ENTRY art_quick_proxy
    mov    r4, lr
    ldr    r5, [sp, #(0 + 4)]
    ldr    r6, [sp, #(0 + 8)]
    ldr    r7, [sp, #(0 + 12)]
    str    r1, [sp, #(0 + 4)]       @this
    str    r2, [sp, #(0 + 8)]       @arg0
    str    r3, [sp, #(0 + 12)]      @arg1
    mov    r3, r0                   @r3 = method
    mov    r0, #1
    push   {r1-r7}
    blx    switchEntry
    pop    {r1-r7}
    mov    ip, r0
    mov    r0, r3
    add    r1, sp, #(0 + 4)
    push   {r1-r7}
    blx    ip
    pop    {r1-r7}
    mov    r2, r0                   @r2 = args
    mov    r0, #2
    push   {r1-r7}
    blx    switchEntry
    pop    {r1-r7}
    mov    r1, r3                   @r1 = method
    str    r5, [sp, #(0 + 4)]
    str    r6, [sp, #(0 + 8)]
    str    r7, [sp, #(0 + 12)]
    ldr    r3, [r0, #METHOD_QUICK_CODE_OFFSET_32]
    mov    ip, r3
    mov    lr, r4
    bx     ip
END art_quick_proxy
