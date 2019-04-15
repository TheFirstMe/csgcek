ASSUME CS:CODE, DS:DATA             ;assuming names to segments

PRINT MACRO MSG                     ;a macro for printing strings
    LEA DX, MSG
    MOV AH, 09H                     ;dos function for displaying string
    INT 21H
ENDM                                ;end of macro definition 

DATA SEGMENT                        ;beginning of data segment
    MSG1 DB "Enter the limit: $"
    MSG2 DB "Enter array elements: $"
    MSG3 DB "Sorted array: $"
    NUM DW 0
    RES DB 100 DUP(?)
DATA ENDS                           ;end of data segment

CODE SEGMENT                        ;beginning of code segment
START: 
        MOV AX,DATA
        MOV DS,AX                   ;move data segment address to DS register
        PRINT MSG1
        CALL READNUM                ;read number of elements to CX
        PRINT MSG2
        MOV NUM,CX                  ;move limit to NUM
        LEA SI,RES                  ;base address of array stored in SI

RARR:   PUSH CX                     ;preserve CX value by pushing to stack
        CALL READNUM                ;read array elements
        MOV [SI],CL                 ;move it to array
        INC SI                      ;point to next element in array
        POP CX                      ;pop counter value from stack
        LOOP RARR
        
        CALL SSORT                  ;perform selection sort
        PRINT MSG3
        LEA SI,RES                  ;base address of array stored in SI
        MOV CX,NUM                  ;move NUM to CX

PARR:   MOV AL,[SI]                 ;move array element to AL
        XOR AH,AH                   ;clear AH register
        CALL PRINTNM                ;print array element
        INC SI                      ;point to next element in array
        LOOP PARR
        
EXIT:   MOV AH,4CH                  ;dos function for termination
        INT 21H
        
READNUM PROC NEAR                   ;procedure to read numbers in numerical representation
        MOV CX,0
        MOV BX,10                   ;multiplier
RD:     MOV AH,01H                  ;dos function for character input 
        INT 21H
        CMP AL,' '                  ;for reading numbers in a single line
        JE EOL
        CMP AL,0DH                  ;checking for carriage return
        JE EOL                      ;end of input/whitespace encountered
        XOR AH,AH                   ;clear AH register
        SUB AL,'0'                  ;convert ASCII character to number
        PUSH AX                     ;push AX to stack
        MOV AX,CX                   ;move CX to AX
        MUL BX                      ;multiply AX by 10
        MOV CX,AX                   ;move it to CX
        POP AX                      ;POP from stack to AX
        ADD CX,AX                   ;add to CX
        JMP RD                      
EOL:    RET                         ;return to calling segmenr
READNUM ENDP 

PRINTNM PROC NEAR                   ;procedure to print number in ASCII format
        PUSH CX                     ;push CX to stack to protect its value
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
        POP CX                      ;pop original value of CX from stack
        RET                         ;return to calling segment
PRINTNM ENDP                        ;end of procedure definition

SSORT   PROC NEAR                   ;procedure to peform selection sort
        MOV CX,NUM                  ;move limit to CX
        CMP CX,1                    ;if only one element present, no need to sort
        JE STOP
        DEC CX                      ;decrement counter
        LEA SI,RES                  ;base address of array stored in SI
OL:     PUSH CX                     ;push outer loop counter to stack
        MOV BX,SI                   ;assume first element in unsorted array is smallest
        MOV DI,SI                   ;pointer for second element in unsorted array
        MOV AX,DI
        SUB AX,OFFSET RES           ;displacement of DI from base address of array
        MOV CX,NUM
        SUB CX,AX                   ;counter for inner loop                    
        DEC CX                      ;decrement counter
IL:     INC DI                      ;increment DI to point to next element in unsorted array
        MOV DL,[DI]                 ;move it to DL
        CMP DL,[BX]                 ;compare it with the assumed minimum value
        JGE SKIP                    ;if lesser than assumed minimum value, update it
        MOV BX,DI                   ;update new minimum element's address
SKIP:   LOOP IL                     ;check for rest of unsorted array
        MOV AL,[BX]                 ;move minimum element in unsorted array to AL
        XCHG [SI],AL                
        MOV [BX],AL                 ;swap it with first element of unsorted array
        POP CX                      ;pop outer loop counter from stack
        INC SI                      ;increment pointer to point to new unsorted array
        LOOP OL        
STOP:   RET                         ;return to calling segment
SSORT   ENDP                        ;end of procedure definition
CODE ENDS                           ;end of code segment
END START                           ;end of program