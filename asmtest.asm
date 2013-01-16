BITS 64

%include "syscall.inc"
%include "asmutils.inc"

SECTION .data
	db_string hw_message, `Hello World\n`

SECTION .bss
	argv: resq 1
	argc: resq 1
	heap: resq 1
	filename: resb	255

SECTION .text
	global _start

_start:
	nop
start:

	pop	qword [argc]
	pop	qword [argv]

;	call	main

;	jmp	.exit


	mov	rax, NR_brk
	xor	rdi,rdi
	syscall

	mov	qword [heap], rax ; preserve a pointer to the heap

	mov	rdi, rax	  ; allocate 4kb on the heap
	adc	rdi, 4096
	mov	rax, NR_brk
	syscall

	mov	rax, 28
	mov	rbx, qword[heap]
	lea	rdi, [rbx+rax]
.loop:
	inc	byte [rbx]
	inc	rbx
	cmp	rbx, rdi
	jne	.loop

	mov	rax,NR_write
	mov	rdi, 1
	mov	rsi,hw_message
	mov	rdx,hw_message.len
	syscall

.exit:
	mov	rax,NR_exit		; put the exit syscall number in eax
	mov	rdi,0
	syscall		; bail out

main:
	stackframe
	.fd:	resq 1
	.heap:	resq 1
	stackend

	push	rbp
	mov	rbp, rsp
	sub	esp, .frame_size

	mov qword [rbp-.fd], qword 0x01
	mov qword [rbp-.heap], -1

	do_syscall write, [rbp-.fd], hw_message, hw_message.len

	mov 	rsp,rbp
	pop	rbp
	ret
strlen:
	; ARGS - address of string on the stack
	; RETURN - length of string on the stack
	pop	rdi
	sub	rcx,rcx
	not	rcx
	sub	rax,rax
	cld
	repne	scasb	
	not	rcx
	dec	rcx
	push	rcx
	ret

; vim: syntax=nasm

