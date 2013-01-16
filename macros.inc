%macro multipush 1-*
%rep  %0
        push %1
%rotate 1
%endrep
%endmacro

%macro multipop 1-*
%rep %0
%rotate -1
        pop     %1
%endrep
%endmacro

%macro sys_write 2

section .data
%%buffer : db %2
%%len : equ $-%%buffer

section .text
%%start :
        multipush rcx, r11
        mov     rax, 1  ; sys_write
        mov     rdi, %1 ; file descriptor
        mov     rsi, %%buffer
        mov     rdx, %%len
        syscall
        multipop rcx, r11

%endmacro


%macro sys_write 3
        multipush rcx, r11
        mov     rax, 1  ; sys_write
        mov     rdi, %1 ; file descriptor
        mov     rsi, %2
%ifidni %1, rdx
        mov     rdx, %3
%endif
        syscall
        multipop rcx, r11
%endmacro

