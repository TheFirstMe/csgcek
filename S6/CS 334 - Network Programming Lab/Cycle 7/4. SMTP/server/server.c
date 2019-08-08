#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define PORT 8888
#define USER_PATH  "./users/"
#define INBOX_PATH "./inboxes/"
// #pragma pack(1)
char response[50];
int flag;
typedef struct{
    char from[50];
    char to[50];
    char subject[50];
    char body[1024];
}Mail;

typedef struct{
    int mailCount;
    Mail mails[100];
}Inbox;

typedef struct{
    char username[50];
    char password[50];
}Creds;

void error(char *);
void recvData(int, void*, int);
void createPath(char *, char *, char *);
void login(Creds);
void createAccount(Creds);
void composeMail(int);
void viewInbox(int, Creds);

int main(){
    struct sockaddr_in serv_addr,cli_addr;
    int sockfd,newsockfd,clilen,enable=1;
    char buffer[256];
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Unable to create socket");
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("Unable to set socket option");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    memset(&cli_addr,0,sizeof(cli_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Unable to bind to socket");
    }

    if(listen(sockfd,5) < 0){
        error("Unable to listen");
    }

    clilen = sizeof(cli_addr);
    if((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0){
        error("Unable to accept");
    }
    int choice;
    Creds creds;
    while(1){
        int flag=0;
        while(1){
            recvData(newsockfd, &choice, sizeof(int));
            if(choice==3){
                flag=1;
                break;
            }
            recvData(newsockfd, &creds, sizeof(Creds));
            switch(choice){
                case 1: 
                        login(creds);
                        break;
                case 2:
                        createAccount(creds);
                        break;
            }
            send(newsockfd, response, sizeof(response),0);
            if(strcmp(response,"success")==0){
                break;
            }
        }
        if(flag==1){
            break;
        }
        flag=0;
        while(1){
            recvData(newsockfd, &choice, sizeof(choice));
            switch(choice){
                case 1:
                        composeMail(newsockfd);
                        break;
                
                case 2:
                        viewInbox(newsockfd, creds);
                        break;
                case 3:
                        break;
                default:
                        break;
            }
            if(choice<1 || choice>4){
                flag = 1;
                break;
            }
            if(choice==3){
                break;
            }
        }
        if(flag==1){
            break;
        }
    }
    return 0;
}

void error(char *message){
    perror(message);
    exit(0);
}

void recvData(int sockfd, void *dest, int size){
  void *buffer,*bfr;
  buffer = malloc(size);
  bfr = buffer;
  int total=0,offset;
  while(total<size){
    offset = recv(sockfd,bfr,size,0);
    if(offset<=0){
        printf("Unknown error\n");
        break;
    }
    total+=offset;
    bfr+=offset;
  }
  memmove(dest,buffer,total);
  free(buffer);
}

void createPath(char *path, char *directory, char *fileName){
    strcpy(path,directory);
    strcat(path, fileName);
}

void login(Creds creds){
    char path[100];
    Creds check;
    createPath(path, USER_PATH, creds.username);
    FILE *user = fopen(path,"rb");
    if(user==NULL){
        strcpy(response,"error1");
        return ;
    }
    fread(&check, sizeof(Creds),1, user);
    if(strcmp(check.password, creds.password)!=0){
        strcpy(response,"error1");
        return ;
    }
    strcpy(response,"success");
    fclose(user);
}

void createAccount(Creds creds){
    char path[100];
    Inbox inbox;
    createPath(path, USER_PATH, creds.username);
    FILE *user = fopen(path,"rb");
    FILE *newuser;
    FILE *inboxptr;
    if(user!=NULL){
        strcpy(response,"error2");
        fclose(user);
        return ;
    }
    newuser = fopen(path,"wb");
    fwrite(&creds,sizeof(Creds),1,newuser);
    fclose(newuser);
    createPath(path, INBOX_PATH, creds.username);
    inboxptr = fopen(path,"wb");
    memset(&inbox,0,sizeof(inbox));
    fwrite(&inbox,sizeof(inbox),1,inboxptr);
    fclose(inboxptr);
    strcpy(response,"success");
}

void composeMail(int sockfd){
    FILE *check;
    char path[100];
    Inbox inbox;
    Mail mail;
    int i;
    recvData(sockfd, &mail, sizeof(Mail));
    createPath(path, INBOX_PATH, mail.to);
    check = fopen(path,"rb");
    if(check==NULL){
        strcpy(response,"error");
        send(sockfd, response, sizeof(response),0);
        return ;
    }
    strcpy(response,"success");
    fread(&inbox,sizeof(inbox),1,check);
    fclose(check);
    send(sockfd, response, sizeof(response),0);
    check = fopen(path,"wb");
    for(i=inbox.mailCount;i>0;i--){
        inbox.mails[i] = inbox.mails[i-1];
    }
    inbox.mails[0] = mail;
    inbox.mailCount++;
    fwrite(&inbox,sizeof(inbox),1,check);
    fclose(check);
}

void viewInbox(int sockfd, Creds creds){
    Inbox inbox;
    char path[100];
    FILE *inboxptr;
    createPath(path, INBOX_PATH, creds.username);
    inboxptr = fopen(path,"rb");
    fread(&inbox,sizeof(inbox),1,inboxptr);
    fclose(inboxptr);
    send(sockfd, &inbox, sizeof(inbox),0);
}