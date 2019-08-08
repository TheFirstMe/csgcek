section .text
    global _start

_start:
    mov eax,4   ;system call for write
    mov ebx,1   ;file descriptor - stdout
    mov ecx,msg ;effective address of msg stored in ecx
    mov edx,len ;length of msg stored in edx
    int 80h     ;call kernel
    
    mov eax,1   ;system call for exit   
    mov ebx,0   ;exit with no error
    int 80h     ;call kernel
    
section .data
msg     db  'Testing Nasm',10   ;message to be printed
len     equ $ - msg             ;length of the message
          
