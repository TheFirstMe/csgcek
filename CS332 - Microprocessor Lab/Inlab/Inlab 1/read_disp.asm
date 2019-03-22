ASSUME CS:CODE, DS:DATA     ;assuming names to logical segments

PRINT MACRO MSG             ;macro for printing strings
    MOV DX, OFFSET MSG
    MOV AH, 09H             ;dos function for printing strings
    INT 21H                 ;dos interrupt
ENDM                        ;end of macro definition

READ MACRO BUF              ;macro for buffered keybaord input
    MOV DX, OFFSET BUF
    MOV AH, 0AH             ;dos function to read buffered input
    INT 21H
    MOV SI, OFFSET BUF+1    ;point to length of string
    MOV CL, [SI]            ;move length to CL and clear CH
    MOV CH,0
    INC CX
    ADD SI, CX              ;increment CX and add to SI to reach at end of string
    MOV AL,"$"
    MOV [SI], AL            ;replace CR by $
ENDM                        ;end of macro definition

DATA SEGMENT                ;data segment begins
    BUFFER DB 21            ;max length of string
    STRLEN DB ?             ;actual length of string
    STRING DB 21 DUP (?)    ;string content
    MSG1 DB 0DH, 0AH, "Enter the string: $"
    MSG2 DB 0DH, 0AH, "The entered string is: $"
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins
START:  MOV AX,DATA
        MOV DS,AX           ;move data segment address to DS
        PRINT MSG1
        READ BUFFER         ;read string to buffer
        PRINT MSG2
        PRINT STRING        ;print input string
        MOV AH, 4CH         ;dos function to terminate the program
        INT 21H
CODE ENDS                   ;code segment ends
END START                   ;end of program