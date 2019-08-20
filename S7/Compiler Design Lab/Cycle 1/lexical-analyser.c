#include<stdio.h>
#include<string.h>
#include<ctype.h>

typedef enum {false, true} bool;

// bool shouldIgnore(char character){
//     char ignore[] = " ,;(){}\n\t";
//     int i, limit = sizeof(ignore)/sizeof(ignore[0]);
//     for(i=0; i < limit; i++){
//         if(character == ignore[i])
//             return true;
//     }
//     return false;
// }

bool isrelop(char a, char b){
    if(a!='>' && a!='<' && a!='!' && a!='=')
        return false;
    else if(a=='=' && b!='=')
        return false;
    else
        return true;
}

bool isarithop(char a){
    char operators[] = "+-*/%";
    int i;
    for(i=0; i < strlen(operators); i++){
        if(operators[i] == a)
            return true;
    }
    return false;
}

bool isreserved(char *token){
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
            return true;
        }
    }
    return false;
}

char* removeComment(char *string, char *type){
    string+=2;
    if(strcmp(type,"//")==0){
        while(*string && *string!='\n')
            string++;
    }
    else{
        while(*string){
            if(*string=='*' && *(string+1)=='/')
                break;
            string++;
        }
        string+=2;
    }
    return string;
}

int gettoken(char *string){
    char str[50],temp[4096],type[10];
    int i=0,j=0, line_no=1;
    while(string[i]!='\0'){
        str[0] = *(string+i), str[1] = *(string+i+1), str[2] = 0;
        if(strcmp(str,"//")==0 || strcmp(str,"/*")==0){
            i = removeComment(string+i,str) - string;
        }
        else{
            temp[j++] = string[i++];
        }
        
    }
    temp[j]='\0';
    i=0,j=0;
    strcpy(str,"");
    while(temp[i]!='\0'){
        //  if(shouldIgnore(temp[i])){
        //     if(temp[i]=='\n')
        //         line_no++;
        //     i++;
        //     continue;
        // }
        if(isrelop(temp[i], temp[i+1])){
            str[0] = temp[i];
            if(temp[i+1]=='=')
                str[1] = '=', str[2] = '\0', i++;
            else
                str[1] = '\0';
            i++;
            strcpy(type,"RELOP");
        }
        else if(isarithop(temp[i])){
            str[0] = temp[i++], str[1]='\0';
            strcpy(type,"ARITH_OP");
        }
        else if(isalpha(temp[i])){
            j=0;
            while(temp[i] && isalnum(temp[i])){
                str[j++] = temp[i++];
            }
            str[j] = '\0';
            if(isreserved(str)){
                strcpy(type,"KEY");
                }
            else
                strcpy(type,"ID");
        }
        else if(isdigit(temp[i])){
            j = 0;
            while(temp[i] && isdigit(temp[i])){
                str[j++] = temp[i++];
            }
            str[j] = '\0';
            strcpy(type,"INT_CONST");
        }
        else{
            if(temp[i]=='\n')
                line_no++;
            i++;
            //unsupported characters
        }
        if(strlen(str) > 0){
            printf("LINE %d <%s, \"%s\">\n",line_no, type, str);
            strcpy(str,"");
        }
    }
}

void main(){
    FILE* file = fopen("sample.c","r");
    char str[4096];
    int d = fread(str,1, sizeof(str), file);
    str[d] = '\0';
    gettoken(str);
    fclose(file);
}