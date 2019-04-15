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
    ADD SI, CX
    INC SI                  ;add CX to SI and increment SI to reach at end of string 
    MOV AL,"$"
    MOV [SI], AL            ;replace CR by $
    DEC SI                  ;point to last character in string
ENDM                        ;end of macro definition

DATA SEGMENT                ;data segment begins
    BUFFER DB 21            ;max length of string
    STRLEN DB ?             ;actual length of string
    STRING DB 21 DUP (?)    ;string content
    REV DB 20 DUP(?)        ;to store reverse of string
    MSG1 DB 0DH, 0AH, "Enter the string: $"
    MSG2 DB 0DH, 0AH, "The entered string is: $"
    MSG3 DB 0DH, 0AH, "The reverse of entered string is: $"
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins
START:  MOV AX,DATA
        MOV DS,AX           ;move data segment address to DS
        PRINT MSG1
        READ BUFFER         ;read string to buffer
        ;SI now points to last character in the string
        PRINT MSG2
        PRINT STRING        ;print the string
        LEA DI, REV         ;store base address of reversed string to DI
R:      MOV AL,[SI]
        MOV [DI],AL         ;move from [SI] to [DI] via AL
        INC DI              ;point to next element in REV
        DEC SI              ;point to previous element in STRING
        LOOP R              ;continue for all letters in STRING
        MOV AL,"$"
        MOV [DI], AL        ;mark end of string for REV
        PRINT MSG3
        PRINT REV           ;print the reversed string
        MOV AH, 4CH         ;dos function to terminate program
        INT 21H
CODE ENDS                   ;code segment ends
END START                   ;end of program