ASSUME CS:CODE, DS:DATA             ;assuming names to segments

PRINT MACRO MSG                     ;a macro for printing strings
    LEA DX, MSG
    MOV AH, 09H                     ;dos function for displaying string
    INT 21H
ENDM                                ;end of macro definition 

DATA SEGMENT                        ;beginning of data segment
    MSG1 DB "Enter the 1st number: $"
    MSG2 DB "Enter the 2nd number: $"
    MSG3 DB "Sum of the numbers: $"
    NUM  DW 0
    SUM  DW 0
DATA ENDS                           ;end of data segment

CODE SEGMENT                        ;beginning of code segment
START:  MOV AX,DATA
        MOV DS,AX                   ;move data segment address to DS register
        PRINT MSG1
        CALL READNUM                ;read first number
        MOV AX,NUM                  ;move it to AX
        ADD SUM,AX                  ;add it to SUM
        PRINT MSG2
        CALL READNUM                ;read second number
        PRINT MSG3                  
        MOV AX,NUM                  ;move it to AX
        ADD AX,SUM                  ;add it to SUM
        CALL PRINTNM                ;print content of AX in ASCII format
EXIT:   MOV AH,4CH                  ;dos function for termination
        INT 21H

READNUM PROC NEAR                   ;procedure to read numbers in numerical representation
        MOV NUM,0
        MOV BX,10                   ;multiplier
RD:     MOV AH,01H                  ;dos function for character input
        INT 21H
        CMP AL,0DH                  ;checking for carriage return
        JE EOL
        XOR AH,AH                   ;clear AH register
        SUB AL,'0'                  ;convert ASCII character to number
        PUSH AX                     ;push AX to stack
        MOV AX,NUM                  ;move NUM to AX
        MUL BX                      ;multiply AX by 10
        MOV NUM,AX                  ;move it to NUM
        POP AX                      ;POP from stack to AX
        ADD NUM,AX                  ;add to NUM
        JMP RD
EOL:    RET
READNUM ENDP                        ;end of procedure definition

PRINTNM PROC NEAR                   ;procedure to print number in ASCII format
        MOV CX,0                    ;for counting number of digits in the number
        MOV BX,10                   ;multipler
TOSTR:  XOR DX,DX                   ;clear DX to store remainder of division
        DIV BX                      ;Divide by 10 to get least significant digit in DX
        ADD DX,'0'                  ;convert it to ASCII format
        PUSH DX                     ;push to stack to get the number in non-reversed format
        INC CX                      ;increment counter
        CMP AX,0                    ;check if quotient is 0
        JG TOSTR
PRTNM:  POP DX                      ;get each digit of the number from stack
        MOV AH,02H                  ;dos function for character output
        INT 21H                     
        LOOP PRTNM
        RET                         ;return to calling segment
PRINTNM ENDP                        ;end of code segment

CODE ENDS                           ;end of code segment
END START                           ;end of program
