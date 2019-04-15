ASSUME CS:CODE, DS:DATA             ;assuming names to segments

PRINT MACRO MSG                     ;a macro for printing strings
    MOV DX, MSG
    MOV AH, 09H                     ;dos function for displaying string
    INT 21H
ENDM                                ;end of macro definition

DATA SEGMENT                        ;beginning of data segment
    MSG1 DB "Enter the string: $"
    MSG2 DB "New string: $"
    MAXLEN EQU 11                   ;maximum length of each word in input string
    STR1 DB 100 DUP(?)              ;to store input string
    LIST DB 20 DUP(MAXLEN DUP(?))   ;a list to store atmost 20 words from input string
    LISTLEN DB 0                    ;number of words in input string
    WORDLEN DB 20 DUP(0)            ;length of each word in input string: 
                                    ;WORDLEN+i gives wordlength of ith word of input string
DATA ENDS                           ;end of data segment

CODE SEGMENT                        ;beginning of code segment
START:  MOV AX,DATA
        MOV DS,AX                   ;move data segment address to DS register
        MOV ES,AX                   ;move segment address to ES register for string instructions
        LEA SI,MSG1                 ;for printing MSG1
        PRINT SI
        LEA SI,STR1                 ;pointing to first position of input string
        LEA DI,LIST                 ;pointing to first position of word list
        LEA BX,WORDLEN              ;pointer to first position of word lenght list
        PUSH DI                     ;push DI content to stack for later use
        MOV CL,1                    ;accounting for $ symbol at end of each word

READ:   MOV AH,01H                  ;dos function for character input
        INT 21H
        CMP AL,0DH                  ;check for carriage return
        JE EOL                      ;if carriage return, input string has ended
        CMP AL,' '                  ;check for whitespace
        JNE W                       
        MOV AL,'$'                  ;if space encountered, replace it by $
        MOV [DI],AL                 ;add $ to DI to denote insertion of new word to list
        MOV [BX],CL                 ;move length of newly inserted word to WORDLEN
        INC BX                      ;increment pointer for storing next word length
        MOV CL,1                    ;reset CL count to 1
        POP DI                      ;restore starting address of newly inserted word to DI
        ADD DI,MAXLEN               ;move pointer MAXLEN times forward to store next word
        PUSH DI                     ;push DI content for getting starting address of next word
        INC LISTLEN                 ;increase length of list
        JMP READ                    ;continue till EOF has reached
W:      MOV [DI],AL                 ;move all non-whitwspace characters to DI
        INC CL                      ;increment word count
        MOV [SI],AL                 ;move input character to SI
        INC SI                      ;point to next position
        INC DI                      ;point to next position
        JMP READ                    ;continue till EOF has reached
EOL:    MOV AL,'$'                 
        MOV [SI],AL                 ;marking end of input string
        MOV [DI],AL                 ;marking end of last word inserted in list
        MOV [BX],CL                 ;word length of last word inserted in list
        
        LEA SI,MSG2                 ;for printing MSG2
        PRINT SI
        LEA SI,LIST                 ;pointing to first word in list
        MOV BH,LISTLEN              ;mov length of list to BH
        LEA BP,WORDLEN              ;pointing to length of first word in list

NXTWRD: LEA DI,LIST                 ;pointing to first word in list
SEARCH: MOV CL,[BP]                 ;move word length to CL for string comparison
        XOR CH,CH                   ;reset CH register
        PUSH SI                     
        PUSH DI                     ;push SI and DI for retrieving their values later
        REPE CMPSB                  ;check if word ponited to by SI is equal to that to DI
        POP DI                      
        POP SI                      ;retrieve values of DI and SI
        JE CHECK                    ;if equal, go to label check
        ADD DI,MAXLEN               ;else, increment DI by MAXLEN to point to next word in string
        JMP SEARCH                  ;continue for all words in list

CHECK:  CMP SI,DI                   ;compare SI and DI.
        JNE NW                      ;if SI!=DI, then don't print the word in SI
        PRINT SI                    ;Else, SI points to a non-duplicate word. Print it.
        MOV DL,' '                  ;for separating words by white space
        MOV AH,02H
        INT 21H

NW:     ADD SI,MAXLEN               ;go to next word in list and check if it is a duplicate
        INC BP                      ;to get the wordlength of the next word
        DEC BH                      ;to check for all words of list
        CMP BH,0
        JL EXIT                     ;if all words are checked, then exit
        JMP NXTWRD                  ;checking if next word is duplicate
        
EXIT:   MOV AH, 4CH                 ;dos function for termination
        INT 21H
CODE ENDS                           ;end of code segment
END START                           ;end of program