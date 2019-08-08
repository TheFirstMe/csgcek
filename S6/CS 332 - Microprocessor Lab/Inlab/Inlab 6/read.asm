section .text
    global _start

%macro read 2
    mov eax,3   ;system call for read
    mov ebx,0   ;file descriptor - stdin
    mov ecx,%1  ;effective address of string stored in ecx
    mov edx,%2  ;maximum length of string stored in edx
    int 80h     ;call kernel
%endmacro

%macro write 2
    mov eax,4   ;system call for write
    mov ebx,1   ;file descriptor - stdout
    mov ecx,%1  ;effective address of string stored in ecx
    mov edx,%2  ;length of string stored in edx
    int 80h     ;call kernel
%endmacro

_start:
    write msg1,len1
    read str1,maxlen    ;read input string
    push eax            ;push length of string to stack
    write msg2,len2
    pop edx             ;pop length of string from stack
    write str1,edx      ;print input string
    
    mov eax,1           ;system call for exit
    mov ebx,0           ;exit with no error
    int 80h             ;call kernel
    
section .data
maxlen  equ 20
msg1    db  'Enter a string: '                
len1    equ $ - msg1                    
msg2    db 'Entered String: '
len2    equ $ - msg2

section .bss
str1    resb 20
