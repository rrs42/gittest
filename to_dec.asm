; Convert 64-bit long int to ASCII
; 
; rc = to_dec( value, buffer, buffer_len )
;
;  value (rdi)
;  buffer (rsi)
;  buffer_len (rdx)
;
;  rc (rax), length of ASCII number or -1 if buffer not long enough
;


%include 'macros.inc'

section .text

global to_dec

to_dec :
	; rdi - value
	; rsi - buffer
	; rdx - buffer_len
	multipush rbx, rbp

	xchg rax, rdi
	xchg rdi, rsi
	mov qword [rsp-8], rdx
	xor rcx, rcx

	xor rbx,rbx
	mov bl, 10

  .digit:
	xor rdx,rdx
	div rbx
	add rdx, BYTE '0'
	mov [rdi+rcx], dl

	test [rsp-8], rcx
	jg .error

	;inc rcx
	add rcx, 1

	test rax, rax
	jnz .digit

	; calculate pointer to end of number string
	lea rsi, [rdi+rcx-1]


	; rcx (length) is the return value
	mov rax, rcx

	; find cx / 2
	sar rcx,1

	; no need to swap if only one digit
	test rcx, rcx
	jz .done

	; loop len/2 times, swapping head and tail
  .swaploop:
	mov bl, byte [rdi]
	xchg bl, byte [rsi]
	mov byte [rdi], bl
	;inc rdi
	add rdi, 1
	;dec rsi
	sub rsi, 1
	loop .swaploop
	jmp .done

  .error:
	mov rax, -1
	
  .done:
	multipop rbx, rbp
	ret

