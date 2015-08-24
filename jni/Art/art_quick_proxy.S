
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

.extern switchEntry
ENTRY art_quick_dispatcher
	push 	{r0-r4}
	mov     r4, lr
	blx     switchEntry
	mov     ip, r0
	mov     lr, r4
	pop     {r0-r4}
    bx      ip
END art_quick_dispatcher
