ASSUME CS:CODE, DS:DATA     ;assuming names to logical segments

PRINT MACRO MSG             ;macro for printing strings
    MOV DX, OFFSET MSG
    MOV AH, 09H             ;dos function for printing strings
    INT 21H                 ;dos interrupt
ENDM                        ;end of macro definition

DATA SEGMENT                ;data segment begins
    MSG1 DB 10,13,"ENTER STRING: $"
    MSG2 DB 10,13,"ENTERED STRING: $"
    MSG3 DB 10,13,"NEW STRING: $"
    VOWELS DB "aeiouAEIOU$" ;list of vowels
    STR DB 40 DUP(?)        ;input string
    NEW DB 40 DUP(?)        ;new string
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins
START:  MOV AX,DATA
        MOV DS,AX           ;move data segment address to DS
        MOV ES,AX           ;also move it to ES to use SCASB instruction
        PRINT MSG1
        LEA SI,STR          ;load base address of STR to SI
        LEA BX,NEW          ;load base address of NEW to BX

READ:   MOV AH,01H          ;dos function to read character to AL
        INT 21H
        CMP AL,0DH          ;check for CR
        JE EOL              ;stop reading if AL=CR
        MOV [SI],AL         ;else, move AL to [SI]
        INC SI              ;point to next element in STR
        LEA DI,VOWELS       ;move base address of vowels list to DI
        MOV CX,10           ;number of vowels
        REPNE SCASB         ;scan for AL in VOWELS
        JE READ             ;if found, skip
        MOV [BX],AL         ;else, move it to [BX]
        INC BX              ;point to next element in NEW
        JMP READ            ;continue till CR is read

EOL:    MOV AL,'$'          ;append $ to STR and NEW
        MOV [SI],AL
        MOV [BX],AL
        PRINT MSG2
        PRINT STR           ;print input string
        PRINT MSG3
        PRINT NEW           ;print updated string	

EXIT:   MOV AH,4CH          ;dos function to terminate program
        INT 21H
CODE ENDS                   ;code segment ends
END START                   ;end of program

