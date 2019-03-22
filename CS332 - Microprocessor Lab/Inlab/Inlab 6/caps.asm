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
    push eax            ;push length to stack
    mov ecx,eax         ;move length to ecx
    dec ecx             ;decrement by 1 to ignore CR
    mov esi,str1        ;move offset address of string to esi
tolower:	
    mov eax,[esi]       ;move value at esi to eax
    cmp al,' '          ;if it is whitespace, ignore
    je skip
    add eax,32          ;else add 32 to convert it to lowercase letter
    mov [esi],eax       ;move the updated value back to esi
skip:	
    inc esi             ;point to next character in the string
    loop tolower        ;continue for all letters in the string
    write msg2,len2
    pop edx             ;pop string length from stack
    write str1,edx      ;print the modified string

    mov eax,1           ;system call for exit
    mov ebx,0           ;exit with no error
    int 80h             ;call kernel
    
section .data
maxlen  equ 20
msg1    db  'Enter a string: '                
len1    equ $ - msg1                    
msg2    db 'New String: '
len2    equ $ - msg2

section .bss
str1    resb 20
lenn    resd 1
