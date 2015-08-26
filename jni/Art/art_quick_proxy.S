
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
	blx    switchEntry
	mov    ip, r0
	mov    lr, r4
	pop    {r0-r4}
	bx     ip
END art_quick_dispatcher

ENTRY art_quick_proxy
    push   {r0-r7}                  @ sp - 32
    mov    r4, lr
    ldr    r5, [sp, #(32 + 4)]
    ldr    r6, [sp, #(32 + 8)]
    ldr    r7, [sp, #(32 + 12)]
    str    r1, [sp, #(32 + 4)]      @this
    str    r2, [sp, #(32 + 8)]      @arg0
    str    r3, [sp, #(32 + 12)]     @arg1
    mov    r3, r0
    mov    r0, #1
    blx    switchEntry
    mov    ip, r0
    mov    r0, r3
    add    r1, sp, #(32 + 4)
    mov    r2, r9
    blx    ip
    mov    r1, r0
    mov    r0, #2
    blx    switchEntry
    mov    ip, r0
    mov    r0, r3
    mov    lr, r4
    str    r5, [sp, #(32 + 4)]
    str    r6, [sp, #(32 + 8)]
    str    r7, [sp, #(32 + 12)]
    pop    {r0-r7}
    bx     ip
END art_quick_proxy
