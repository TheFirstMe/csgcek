#include<stdio.h>
#include<string.h>
#include<ctype.h>

//size of each buffer
#define N 20
//a portable version of bzero for clearing buffer
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

//file pointer to input file
FILE *file;

//implenting a 2-buffer scheme
char buffer[2*N]={0};
char *token_beginning = buffer;
char *forward = buffer;
int start=0;
int state=0;
int d=-1;
//line counter
int linecnt=1;
//for storing token values
char lexical_value[20];

//a structure to store each token's name, value and the line number it has occurred
typedef struct Token{
    char name[50];
    char value[20];
    int line;
}Token;

//to recover from analysis of unsupported tokens
void recover(){
        //reset start state
        start = 0;
        //skip the problematic character
        token_beginning++;
        //set forward pointer to token_beginning to get next valid token
        forward=token_beginning;
        //checking if token_beginning has crossed buffer boundaries
        if(token_beginning==buffer+N-1)
                token_beginning = buffer+N;
        else if(token_beginning==buffer+2*N-1){
                token_beginning = buffer;
        }
}

//for moving onto next set of DFA states if current set of states is not applicable
int fail(){
    switch(start){
        //current character does not denote a relational operator
        //so move on to check for identifiers/keys
        case 0: start=12;
                break;
        //current character does not denote an identifier or a key
        //so move on to check for integer constants                
        case 12:
                start=15;
                break;
        //current character does not denote an interger constant
        //so move on to check for arithmetic operators
        case 15:
                start=18;
                break;
        //current character does not denote an arithmetic operator, or any valid tokens
        //so recover by skipping current character 
        case 18: 
                recover();
                break;
    }
    //return new start state
    return start;
}

//for retrieving token from the buffer
void getValue(){
    //token starts from token_beginning pointer
    char *ptr=token_beginning;
    int i=0;
    if(ptr<forward){
        //beginning of token resides in first buffer
        while(ptr<forward){
            if(ptr==buffer+N-1){
                //if token end is in the second buffer; skip EOF of first buffer
                ptr++;
                continue;
            }
            //copy token to a global variable to "return" it
            lexical_value[i++]=*ptr++;
        }
    }
    else{
        //beginning of token resides in second buffer
        //if token end is in the first buffer; skip EOF of second buffer
        while(ptr!=buffer+2*N-1){
            lexical_value[i++]=*ptr++;
        }
        ptr=buffer;
        while(ptr<forward){
            lexical_value[i++]=*ptr++;
        }
    }
    lexical_value[i]='\0';
}

//for checking if token generated is an identifier
int isReserved(char *token){
    char reserved[][10] = {
        "auto",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "int",
        "long",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if"
    };
    int reslen = sizeof(reserved)/sizeof(reserved[0]);
    int i;
    for(i=0;i<reslen;i++){
        if(strcmp(reserved[i],token)==0){
            return 1;
        }
    }
    return 0;
}

//retrieve next character from buffer; makes use of lookahead technique
int nextChar(){
    switch(*forward){
        //has reached end of buffer or end of input file
        case EOF:
                if(forward==buffer+N-1){
                    //reached end of first buffer
                    //set forward pointer to second buffer
                    forward = buffer + N;
                    //clear second buffer
                    bzero(buffer+N,N-2);
                    //fill second buffer
                    d = fread(buffer+N,1,N-1,file);
                    if(d!=N-1){
                        //number of characters read is less than max. size of each buffer
                        //so input file is consumed completely
                        //put EOF to mark end of file
                        (buffer+N)[d]=EOF;
                    }
                }
                else if(forward==buffer+(2*N)-1){
                    //reached end of second buffer
                    //set forward pointer to first buffer
                    forward = buffer;
                    //clear first buffer
                    bzero(buffer,N-2);
                    //fill first buffer
                    d = fread(buffer,1,N-1,file);
                    if(d!=N-1){
                        //number of characters read is less than max. size of each buffer
                        //so input file is consumed completely
                        //put EOF to mark end of file
                        buffer[d]=EOF;
                    }
                }
                else{
                //We have reached end of input file. No action to take here
                }
        default: 
                //return a character from the buffer and increment the forward pointer
                return *forward++;
    }
}

//retract forward pointer due to lookahead process
void retract(){
    if(forward==buffer){
        forward=buffer+(2*N)-2;
    }
    else if(forward==buffer+N){
        forward=buffer+N-2;
    }
    else{
        forward--;
    }
}

//return next token in input file, NULL if reached EOF
void *nexttoken(){
    //declared as static to keep its existence throughout the program
    //so it's addressed can be returned to main function
    static Token token;
    //set start and current state to 0 to identify next token
    start=state=0;
    //for retrieving a character from the buffer
    int c;
    //set token_beginning to forward pointer if it does not point to buffer boundaries
    if(forward==buffer+N-1){
        token_beginning=buffer+N;
    }
    else if(forward==buffer+2*N-1){
        token_beginning=buffer;
    }
    else{
        token_beginning=forward;
    }
    //infinite loop until a valid token is found
    while(1){
        switch(state){
            case 0: //retrieve a character from buffer
                    c = nextChar();
                    if(c==EOF){
                        //reached end of input file
                        return NULL;
                    }
                    else if(c==' '||c=='\n' || c=='\t'){
                        if(c=='\n'){
                            //encountered a new line, so increment the line counter
                            linecnt++;
                        }
                        //keep skipping whitespace,tabspace and newline by setting state to 0
                        state = 0;
                        //increment token_beginning and forward to point to new character
                        token_beginning++;
                        forward=token_beginning;
                        //checking if token_beginning has crossed buffer boundaries
                        if(token_beginning==buffer+N-1){
                                token_beginning = buffer+N;
                        }
                        else if(token_beginning==buffer+2*N-1){
                            token_beginning = buffer;
                        }
                    }
                    else if(c=='<'){
                        //either a <= or < token
                        state=1;
                    }
                    else if(c=='='){
                        //either a == or = symbol (invalid)
                        state=4;
                    }
                    else if(c=='>'){
                        //either a >= or > token
                        state=7;
                    }
                    else if(c=='!'){
                        //either a != or ! symbol (invalid)
                        state=10;
                    }
                    else{
                        //not a relational operator
                        state=fail();
                    }
                    break;
            case 1: c = nextChar();
                    if(c=='='){
                        //token is <=
                        state=2;
                    }
                    else{
                        //token is <
                        state=3;
                    }
                    break;
            case 2: strcpy(token.name,"RELOP");
                    strcpy(token.value,"\"<=\"");
                    token.line=linecnt;
                    return &token;
            case 3: //retract because of lookahead in case 1
                    retract();
                    strcpy(token.name,"RELOP");
                    strcpy(token.value,"\"<\"");
                    token.line=linecnt;
                    return &token;
            case 4: c = nextChar();
                    if(c=='='){
                        //token is ==
                        state=5;
                    }
                    else{
                        //symbol is =; invalid
                        state=6;
                    }
                    break;
            case 5: strcpy(token.name,"RELOP");
                    strcpy(token.value,"\"==\"");
                    token.line=linecnt;
                    return &token;
            case 6: //retract because of lookahead in case 4 and case 10
                    retract();
                    //skip the character
                    token_beginning=forward;
                    //set start and current state to 0 to check for next valid token
                    start=state=0;
                    break;
            case 7: c = nextChar();
                    if(c=='='){
                        //token is >=
                        state=8;
                    }
                    else{
                        //token is >
                        state=9;
                    }
                    break;
            case 8: strcpy(token.name,"RELOP");
                    strcpy(token.value,"\">=\"");
                    token.line=linecnt;
                    return &token;
            case 9: //retract because of lookahead in case 7
                    retract();
                    strcpy(token.name,"RELOP");
                    strcpy(token.value,"\">\"");
                    token.line=linecnt;
                    return &token;
            case 10: 
                    c = nextChar();
                    if(c=='='){
                        //token is !=
                        state=11;
                    }
                    else{
                        //symbol is !; invalid
                        state=6;
                    }
                    break;
            case 11:
                    strcpy(token.name,"RELOP");
                    strcpy(token.value,"\"!=\"");
                    token.line=linecnt;
                    return &token;
            case 12:
                    if(isalpha(c)){
                        //token is an identifier or key
                        state=13;
                    }
                    else{
                        //not an identifier or a key
                        state=fail();
                    }
                    break;
            case 13:
                    c = nextChar();
                    //retrieve the token
                    if(isalpha(c)){
                        state=13;
                    }
                    else if(isdigit(c)){
                        state=13;
                    }
                    else{
                        //end of the token
                        state=14;
                    }
                    break;
            case 14:
                    //retract because of lookahead in 13
                    retract();
                    //retrieve token from buffer
                    getValue();
                    if(isReserved(lexical_value)){
                        //token is a key
                        strcpy(token.name,"KEY");
                    }
                    else{
                        //token is an identifier
                        strcpy(token.name,"ID");
                    }
                    strcpy(token.value,lexical_value);
                    token.line=linecnt;
                    return &token;
            case 15:
                    if(isdigit(c)){
                        //token is an integer constant
                        state=16;
                    }
                    else{
                        //not an integer constant
                        state=fail();
                    }
                    break;
            case 16:
                    c=nextChar();
                    //retrieve the token
                    if(isdigit(c)){
                        state=16;
                    }
                    else{
                        //end of token
                        state=17;
                    }
                    break;
            case 17:
                    //retract because of lookahead in case 16
                    retract();
                    getValue();
                    strcpy(token.name,"INT_CONST");
                    strcpy(token.value,lexical_value);
                    token.line=linecnt;
                    return &token;
            case 18:
                    if(c=='+'){
                        //token is +
                        state=19;
                    }
                    else if(c=='-'){
                        //token is -
                        state=20;
                    }
                    else if(c=='*'){
                        //token is *
                        state=21;
                    }
                    else if(c=='/'){
                        //token is /
                        state=22;
                    }
                    else if(c=='%'){
                        //token is %
                        state=23;
                    }
                    else{
                        //invalid token!!
                        state=fail();
                    }
                    break;
            case 19:
                    strcpy(token.name,"ARITH_OP");
                    strcpy(token.value,"+");
                    token.line=linecnt;
                    return &token;
            case 20:
                    strcpy(token.name,"ARITH_OP");
                    strcpy(token.value,"-");
                    token.line=linecnt;
                    return &token;
            case 21:
                    strcpy(token.name,"ARITH_OP");
                    strcpy(token.value,"*");
                    token.line=linecnt;
                    return &token;
            case 22:
                    strcpy(token.name,"ARITH_OP");
                    strcpy(token.value,"/");
                    token.line=linecnt;
                    return &token;
            case 23:
                    strcpy(token.name,"ARITH_OP");
                    strcpy(token.value,"%");
                    token.line=linecnt;
                    return &token;
            default:
                    printf("\nError!");
                    return NULL;
        }
    }
}

int main(){
    Token *token;
    //storing all tokens
    FILE *result = fopen("pooy.txt","w");
    //marking EOF for first buffer
    buffer[N-1] = EOF;
    //marking EOF for second buffer
    buffer[2*N-1] = EOF;
    //open input file
    file = fopen("aloy.c","r");
    //clear first buffer
    bzero(buffer,N-1);
    //fill first buffer
    d=fread(buffer,1,N-1,file);
    if(d!=N-1){
        //buffer is not full. Mark EOF of input
        buffer[d]=EOF;
    }
    while(token=nexttoken()){
        fprintf(result,"Line %d <%s, %s>\n",token->line,token->name,token->value);
    }
    //close file handler
    fclose(file);
    fclose(result);
    return 0;
}