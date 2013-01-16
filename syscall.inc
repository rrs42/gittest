%ifndef __SYSCALL_INC__
%define __SYSCALL_INC__

; Syscalls


%define NR_read		0
%define NR_write	1
%define NR_open		2
%define NR_close	3
%define NR_stat		4
%define NR_brk		12
%define NR_exit		60

; X86_64 kernel calling convention
; rax : syscall number
; rdi, rsi, rdx, r10, r8, r9
; result is in rax
; rcx, r11 not preserved after call


%imacro do_syscall 1-6
 %if %0 > 1
  %if %0 > 2
   %if %0 > 3
    %if %0 > 4
     %if %0 > 5
      %if %0 > 6
       mov r9, %{7}
      %endif
      mov r8, %{6}
     %endif
     mov r10, %{5}
    %endif
    mov rdx, %{4}
   %endif
   mov rsi, %{3}
  %endif
  mov rdi, %{2}
 %endif
 mov rax, NR_ %+ %{1}
 syscall
%endmacro



%endif ; __SYSCALL_INC__
; vim: syntax=nasm

