ASSUME CS:CODE, DS:DATA             ;assuming names to segments

PRINT MACRO MSG                     ;a macro for printing strings
    LEA DX, MSG
    MOV AH, 09H                     ;dos function for displaying string
    INT 21H
ENDM                                ;end of macro definition

DATA SEGMENT                        ;beginning of data segment
    MSG1 DB "Enter the limit: $"
    MSG2 DB "Prime numbers: $"
    NUM DW 0
DATA ENDS                           ;end of data segment

CODE SEGMENT                        ;beginning of code segment
START: 
        MOV AX,DATA					
        MOV DS,AX                   ;move data segment address to DS register
        PRINT MSG1
        CALL READNUM                ;read the limit to CX
        PRINT MSG2
        CALL PRIME                  ;procedure to print first n prime numbers
		
EXIT:   MOV AH,4CH                  ;dos function for termination
        INT 21H
		
READNUM PROC NEAR                   ;procedure to read numbers in numerical representation
        MOV CX,0                    ;move 0 to CX
        MOV BX,10                   ;multiplier
RD:     MOV AH,01H                  ;dos function for character input
        INT 21H
        CMP AL,0DH                  ;checking for carriage return
        JE EOL                      ;end of input
        XOR AH,AH                   ;clear AH register
        SUB AL,'0'                  ;convert ASCII character to number
        PUSH AX                     ;push AX to stack
        MOV AX,CX                   ;move AX to CX
        MUL BX                      ;multiply AX by 10
        MOV CX,AX                   ;move result to CX
        POP AX                      ;pop from stack to AX
        ADD CX,AX                   ;add AX to CX
        JMP RD                      ;cotinue till EOL
EOL:    RET

READNUM ENDP                        ;end of procedure definition

PRINTNM PROC NEAR                   ;procedure to print number in ASCII format
        PUSH AX	
        PUSH CX                     ;push AX and CX to stack to protect their values
        MOV CX,0                    ;for counting number of digits in the number
        MOV BX,10
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
        LOOP PRTNM                  ;print all digits
        MOV DL,' '                  ;for printing a space
        MOV AH,02H                  ;dos function for character output
        INT 21H
        POP CX
        POP AX                      ;pop original values of CX and AX from stack
        RET                         ;return to calling segment
PRINTNM ENDP                        ;end of procedure definition

PRIME   PROC NEAR                   ;procedure to print first n prime numbers
        CMP CX,1                    ;check if the limit is less than one
        JL ERR
        MOV AX,2                    ;first prime number is 2
        JMP PRIME2                  ;no need to check for this
NXTNUM: INC AX                      ;next potential prime number
        MOV BX,2                    ;for checking if number in AX has any factor 
CHECK:  PUSH AX                     ;push AX to stack
        XOR DX,DX                   ;clear DX to store remainder of division
        DIV BX                      ;divide AX by BX
        POP AX                      ;pop from stack to AX
        CMP DX,0                    ;check if BX is a factor of AX
        JE NXTNUM                   ;if yes, check for next number
        INC BX						
        CMP BX,AX                   ;check if BX=AX
        JL CHECK                    ;if not, continue checking primality of AX
PRIME2: CALL PRINTNM                ;else, AX is a prime number; print it.
        LOOP NXTNUM
ERR:    RET                         ;return to calling segment
PRIME ENDP                          ;end of procedure definition
CODE ENDS                           ;end of code segment
END START                           ;end of program