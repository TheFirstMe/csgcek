ASSUME CS:CODE, DS:DATA     ;assume names to logical segments

PRINT MACRO MSG             ;macro to print strings
    LEA DX, MSG
    MOV AH, 09H             ;dos function to print strings
    INT 21H
ENDM                        ;end of macro definition

DATA SEGMENT                ;data segment begins
    MSG1 DB "Enter number of rows: $"
    MSG2 DB "Enter number of columns: $"
    MSG3 DB "Enter matrix elements: ",10,13,"$"
    MSG4 DB  10,13,"Spiral of matrix: $"
    ROW DW ?
    COL DW ?
    MAX EQU 10
    RES DB MAX DUP(MAX DUP(?))
DATA ENDS                   ;data segment ends

CODE SEGMENT                ;code segment begins
START: 
        MOV AX,DATA         ;move data segment address to DS
        MOV DS,AX
        MOV ES,AX           ;also move it to ES to use STOSB
        PRINT MSG1
        CALL READNUM        ;read number of rows
        MOV ROW,AX
        PRINT MSG2
        CALL READNUM        ;read number of columns
        MOV COL,AX
        PRINT MSG3
        LEA DI,RES          ;base address of matrix stored in DI
        MOV CX,ROW          ;set outer loop variable to ROW
        MOV DH,1            ;set flag to 1

OLOOP:  PUSH CX             ;push CX to stack to use CX for inner loop
        MOV CX,COL          ;set inner loop variable to COL
RLOOP:  CALL READNUM        ;read matrix element
        STOSB               ;store it to [DI] and point to next/prev element
        LOOP RLOOP          ;continue for all elements in current row
        CMP DH,1            ;check if row is to be reversed
        JNE NREV
        MOV DH,0            ;if DH=1, reverse the row
        STD                 ;set direction flag to auto decrement DI
        DEC DI              ;decrement DI to point to last element in the next row
        JMP REV
NREV:   MOV DH,1            ;if DH!=1,don't reverse the row
        CLD                 ;clear the direction flag
        INC DI              ;increment DI to point to first element in the next row
REV:    ADD DI,COL          ;increment DI to point to next row
        POP CX              ;restore outer loop variable from stack
        LOOP OLOOP          ;continue for all rows in matrix
            
        PRINT MSG4
        CALL SPIRAL         ;print spiral of the matrix
            
EXIT:   MOV AH,4CH          ;dos function to exit the program
        INT 21H
        
READNUM PROC NEAR
        PUSH DX             ;push DX and CX to stack to protect their values
        PUSH CX
        MOV CX,0            ;move 0 to CX
        MOV BX,10           ;multiplier
RD:     MOV AH,01H          ;dos function to read a character
        INT 21H
        CMP AL,' '          ;skips spaces
        JE EOL
        CMP AL,0DH          ;check for CR
        JE EOL
        XOR AH,AH           ;clear AH
        SUB AL,'0'          ;convert ASCII character number
        PUSH AX             ;push AX to stack
        MOV AX,CX           ;move CX to AX
        MUL BX              ;multiply AX by 10
        MOV CX,AX           ;move AX to CX
        POP AX              ;pop from stack to AX
        ADD CX,AX           ;add AX to CX
        JMP RD              ;continue until CR or space
EOL:    MOV AX,CX           ;move CX to AX
        POP CX              ;restore original values of CX and DX from stack
        POP DX
        RET                 ;return to calling segment
READNUM ENDP                ;end of procedure definition

PRINTNM PROC NEAR           ;procedure to print number in ASCII format
        PUSH CX             ;push CX,DX to protect their values
        PUSH DX
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
        LOOP PRTNM          ;continue till all digits are printed
        MOV DL,' '          ;print a space
        MOV AH,02H
        INT 21H
        POP DX              ;restore original values of DX and CX
        POP CX
        RET                 ;return to calling segment
PRINTNM ENDP                ;end of procedure definition 

SPIRAL  PROC NEAR           ;procedure to print spiral of a matrix
        LEA SI,RES          ;base address of matrix stored in SI
        CLD                 ;clear direction flag
        MOV CX,ROW          ;set outer loop variable to ROW
OLOOP2: PUSH CX             ;push CX to stack to use CX in inner loop
        MOV CX,COL          ;set inner loop variable to COL
ILOOP2: LODSB               ;move element to AL and point to next element
        XOR AH,AH           ;clear AH
        CALL PRINTNM        ;print the element
        LOOP ILOOP2         ;continue for all elements in current row
        POP CX              ;restore outer loop variable from stack
        LOOP OLOOP2         ;continue for all rows in the matrix
        RET                 ;return to calling segment
SPIRAL  ENDP                ;end of procedure definition
CODE ENDS                   ;end of code segment
END START                   ;end of program