ASSUME CS:CODE, DS:DATA     ;assuming names to logical segments

PRINT MACRO MSG             ;macro for printing strings
    MOV DX, OFFSET MSG
    MOV AH, 09H             ;dos function for printing strings
    INT 21H                 ;dos interrupt
ENDM                        ;end of macro definition

READCHAR MACRO              ;macro for reading a character
    MOV AH,01H              ;dos function to read character to AL
    INT 21H
ENDM
    
DATA SEGMENT                ;data segment begins
    MSG1 DB 10,13,"ENTER STRING: $"
    MSG2 DB 10,13,"ENTER CHARACTER TO BE SEARCHED: $"
    MSG3 DB 10,13,"CHARACTER FOUND AT POSITION: $"
    MSG4 DB 10,13,"CHARACTER NOT FOUND$"
    STR DB 20 DUP(?)        ;input string
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins
START:  MOV AX,DATA
        MOV DS,AX           ;move data segment address to DS
        MOV ES,AX           ;also move it to ES to use SCASB instruction
        PRINT MSG1
        LEA SI,STR          ;load base address of STR to SI
        MOV BL,00H          ;set BL to 0
READ:   READCHAR            ;read a character
        CMP AL,0DH          ;check if it is CR
        JE EOL              ;if it is, stop reading
        MOV [SI],AL         ;else, move it to [SI]
        INC SI              ;point to next element in STR
        INC BL              ;increment string length
        JMP READ            ;continue till CR is read
EOL:    MOV AL,'$'          ;append $ to STR
        MOV [SI],AL
        PRINT MSG2
        LEA DI,STR          ;load base address of STR to DI
        READCHAR            ;read character to be searched
        MOV CL,BL           ;move string length to CX
        XOR CH,CH
        REPNE SCASB         ;check for AL in STR
        JNE NFOUND
        SUB BL,CL           ;if found, subtract CL from BL to find character position
FOUND:  PRINT MSG3
        ADD BL,'0'          ;convert number to ASCII format
        MOV DL,BL           ;move it to DL
        MOV AH,02H          ;dos function to print character stored in DL
        INT 21H
        JMP EXIT
NFOUND: PRINT MSG4          ;AL not found in STR
EXIT:   MOV AH,4CH          ;dos function to terminate the program
        INT 21H
CODE ENDS                   ;code segment ends
END START                   ;end of program