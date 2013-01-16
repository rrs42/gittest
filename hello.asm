; 64-bit "Hello World!" in Linux NASM

section .text
global _start            ; global entry point export for ld

%include "asmutils.inc"

_start:

	mov rdi, [value1]
	mov rsi, buffer
	mov rdx, buffer_len
	call to_dec

	mov rdx, rax
	sys_write 1, buffer, rdx
        sys_write 1, `\n`

exit:
        ; sys_exit(return_code)
        mov    rax, 60        ; sys_exit
        mov    rdi, 0        ; return 0 (success)
        syscall


error :
        sys_write 1, `Stack Error\n`
        ret

to_dec :
	; rdi - value
	; rsi - buffer
	; rdx - buffer_len

	xchg  rax, rdi
	xchg  rdi, rsi
	mov qword [rsp-8], rdx
	xor rcx, rcx

	mov  rbx, 10

  .digit:
	xor rdx,rdx
	div rbx
	add rdx, '0'
	mov [rdi+rcx], dl
	inc rcx
	test rax, rax
	jnz .digit

	; calculate pointer to end of number string
	lea rsi, [rdi+rcx-1]


	; rcx (length) is the return value
	mov rax, rcx

	; find cx / 2
	sar rcx,1

	; loop len/2 times, swapping head and tail

  .swaploop:
	mov al, byte [rdi]
	xchg al, byte [rsi]
	mov byte [rdi], al
	inc rdi
	dec rsi
	loop .swaploop
	
	ret


section .bss
        argc: resq 1
        argv: resq 1
        buffer: resb 32
        buffer_len : equ $ - buffer

section .data
value1: dq 0xDEADBEEFDEADBEEF
value2: dq 0xFFFFFFFFFFFFFFFF
value3: dq 1234
value4: dq 12345

