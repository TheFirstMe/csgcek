ASSUME CS:CODE, DS:DATA     ;assuming names to logical segments

PRINT MACRO MSG             ;macro for printing strings
        MOV DX, OFFSET MSG
        MOV AH, 09H         ;dos function for printing strings
        INT 21H             ;dos interrupt
ENDM                        ;end of macro definition

DATA SEGMENT                ;data segment begins
        MSG1 DB 10,13,"Enter the string: $"
        MSG2 DB 10,13,"The string is: $"
        MSG3 DB 10,13,"Reversed string is: $"
        MSG4 DB 10,13,"The string is palindrome $"
        MSG5 DB 10,13,"The string is not a palindrone $"
        MSG6 DB 10,13,"The palindrome string is $"
        STR  DB 20 dup(?)   ;input string
        REV  DB 20 dup(?)   ;reversed string
        PAL  DB 40 dup(?)   ;input string + reversed string
        VAR  DB ?
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins

START:                      ;start of program
        MOV AX,DATA
        MOV DS,AX           ;move data segment address to DS
        MOV CX,00H          ;set CX to 0
        PRINT MSG1
        LEA SI,STR          ;load base address of STR to SI
L2:
        MOV AH,01h          ;dos function to read a charactet to AL
        INT 21H
        CMP AL,0DH          ;check for CR
        JE L1               ;stop reading if AL=CR
        MOV [SI],AL         ;else, move AL to [SI]
        INC SI              ;point to next element in STR
        INC CX              ;increment string length
        JMP L2              ;continue until CR is read
L1:     MOV AL,'$'          ;append $ to STR
        MOV [SI],AL
        LEA BX,VAR
        MOV BX,CX           ;move string length to VAR
        LEA DI,REV          ;load base address of REV to DI
        PRINT MSG2
        PRINT STR           ;print input string
L3:     DEC SI              ;start from end of STR and decrement
        MOV AL,[SI]
        MOV [DI],AL         ;move [SI] to [DI] via AL
        INC DI              ;point to next element in REV
        LOOP L3             ;continue for all elements in STR
        MOV CX,BX           ;move string length to BX
        MOV AL,'$'          ;append $ to REV
        MOV [DI],AL
        PRINT MSG3
        PRINT REV           ;print the reversed string
        LEA SI,STR          ;load base address of STR to SI
        LEA DI,REV          ;load base address of REV to DI
L4:     MOV AL,[SI]
        MOV DL,[DI]
        CMP AL,DL           ;compare [SI] and [DI] through AL and DL
        JNE L5              ;check if [SI]=[DI]
        INC SI
        INC DI              ;if yes, point to next elements in STR and REV
        LOOP L4             ;check for all elements in STR and REV
        PRINT MSG4          ;if control reached here => STR is a palindrome
        JMP EXIT
L5:     PRINT MSG5          ;if control reached here => STR is not a palindrome
        LEA SI,STR          ;load base address of STR to SI
        LEA DI,PAL          ;load base address of PAL to DI
        MOV CX,BX           ;move string length to CX
L6:     MOV AL,[SI]         ;move from [SI] to [DI] via AL
        MOV [DI],AL
        INC SI              ;point to next elements in STR and PAL
        INC DI
        LOOP L6             ;continue for all elements in STR
        MOV CX,BX           ;move string length to CX
        LEA SI,REV          ;load base address of REV to SI
L7:     MOV AL,[SI]         ;move from [SI] to [DI] via AL
        MOV [DI],AL
        INC SI              ;point to next elements in REV and PAL
        INC DI
        LOOP L7             ;continue for all elements in REV
        MOV AL,'$'          ;append $ to PAL
        MOV [DI],AL
        PRINT MSG6
        PRINT PAL           ;print the palindrom string
EXIT:   MOV AH,4CH          ;dos function to terminate the program
        INT 21H
CODE ENDS                   ;code segment ends
END START                   ;end of program