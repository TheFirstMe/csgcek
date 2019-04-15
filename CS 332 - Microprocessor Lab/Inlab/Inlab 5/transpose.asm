ASSUME CS:CODE, DS:DATA     ;assume names to logical segments

PRINT MACRO MSG             ;macro to print strings
    LEA DX, MSG
    MOV AH, 09H             ;dos function to print strings
    INT 21H
ENDM                        ;end of macro definition

DATA SEGMENT                ;data segment begins
    MSG1 DB "Enter order of matrix: $"
    MSG2 DB "Enter matrix elements: ",10,13,"$"
    MSG3 DB 10,13,"Transpose: ",10,13,"$"
    ORDER DW 0
    MAX EQU 10
    RES DB MAX DUP(MAX DUP(?))
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins
START:                      ;start of program
        MOV AX,DATA         ;move address of data segment to DS
        MOV DS,AX
        PRINT MSG1
        CALL READNUM        ;read order of the matrix
        MOV ORDER,DX        ;move it to ORDER
        MOV CX,ORDER        ;move ORDER to CX
        PRINT MSG2
        LEA SI,RES          ;store base address of matrix to SI

OLOOP1: PUSH SI             ;SI should point to first element in each column
        PUSH CX             ;push CX to stack to use CX in inner loop
        MOV CX,ORDER        ;move ORDER to CX
ILOOP1: CALL READNUM        ;read matrix element to DX
        MOV [SI],DL
        ADD SI,ORDER        ;point to next element in current column
        LOOP ILOOP1         ;continue for all elements in current row
        POP CX              ;restore outer loop counter from stack
        POP SI              ;retrieve pointer of first element of current column
        INC SI              ;point to first element in next column
        LOOP OLOOP1         ;continue for all rows in the matrix
                
        PRINT MSG3
        CALL PMTRX          ;print transpose of the matrix
        
EXIT:   MOV AH,4CH          ;dos function to terminate the program
        INT 21H
        
READNUM PROC NEAR           ;procedure to read numbers in numerical representation
        MOV DX,0            ;move 0 to DX
        MOV BX,10           ;multiplier
RD:     MOV AH,01H          ;dos function to read a character
        INT 21H
        CMP AL,' '          ;skips spaces
        JE EOL
        CMP AL,0DH          ;check for CR
        JE EOL
        XOR AH,AH           ;clear AH register
        SUB AL,'0'          ;convert ASCII character to number
        PUSH AX             ;push AX to stack
        MOV AX,DX           ;move DX to AX
        MUL BX              ;multiply AX by 10
        MOV DX,AX           ;move AX to DX
        POP AX              ;pop from stack to AX
        ADD DX,AX           ;add AX to DX
        JMP RD              ;continue till CR or space
EOL:    RET                 ;return to calling segment
READNUM ENDP                ;end of procedure definition

PRINTNM PROC NEAR           ;procedure to print number in ASCII format
        PUSH CX             ;push CX to protect its value
        MOV CX,0            ;move 0 to CX
        MOV BX,10           ;multiplier
TOSTR:  XOR DX,DX           ;clear DX to store remainder
        DIV BX              ;divide AX by BX
        ADD DX,'0'          ;convert digit to ASCII
        PUSH DX             ;push it to stack
        INC CX              ;increment counter
        CMP AX,0            ;check if all digits are retrieved
        JG TOSTR            ;continue till all digits are pushed to stack
PRTNM:  POP DX              ;pop digit from stack in reverse order 
        MOV AH,02H          ;dos function to print character
        INT 21H             
        LOOP PRTNM              
        MOV DL,' '          ;print a space 
        MOV AH,02H              
        INT 21H             
        POP CX              ;restore original value of CX
        RET                 ;return to calling segment
PRINTNM ENDP                ;end of procedure definition

PMTRX   PROC NEAR           ;procedure to print matrix elements
        LEA SI,RES          ;store base address of matrix to be printed
        MOV CX,ORDER        ;store order of matrix to CX
OLOOP2: PUSH CX             ;push CX to stack to modify it in inner loop
        MOV CX,ORDER        ;set CX to ORDER for inner loop iteration
ILOOP2: MOV AL,[SI]         ;move matrix element to AL
        XOR AH,AH           ;clear AH
        CALL PRINTNM        ;print the element
        INC SI              ;point to next element
        LOOP ILOOP2         ;continue for all elements in current row
        MOV DL,10           ;print a new line character indicating next row
        MOV AH,02H
        INT 21H
        POP CX              ;restore outer loop counter
        LOOP OLOOP2         ;continue for all rows in the matrix
        RET                 ;return to calling segment
PMTRX   ENDP                ;end of procedure definition
CODE ENDS                   ;end of code segment
END START                   ;end of program
