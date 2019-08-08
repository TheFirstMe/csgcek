#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>

#define PORT 8888

char response[50];

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
void recvData(int ,void *, int);
int mainmenu();
Creds account(int);
int mailOptions();
int composeMail(int ,char *);
void viewInbox(int);


int main(){
    struct sockaddr_in serv_addr;
    int sockfd;
    char buffer[256];
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Unable to create socket");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Unable to connect to server");
    }

    Creds creds;
    int choice;
    while(1){
        int flag=0;
        while(1){
            choice = mainmenu();
            if(choice<1 || choice>3){
                printf("Incorrect option!!\n");
                getchar();
                continue;
            }
            send(sockfd, &choice, sizeof(choice),0);
            if(choice==3){
                flag=1;
                break;
            }
            switch(choice){
                case 1:
                case 2:
                        creds = account(sockfd);
                        break;
            }
            send(sockfd, &creds, sizeof(creds),0);
            recvData(sockfd, response, sizeof(response));
            if(strcmp(response,"success")!=0){
                if(strcmp(response,"error1")==0)
                    printf("Invalid username or password!\n");
                else
                    printf("User already exists!\n");
                getchar();
                continue;
            }
            break;
        }
        if(flag==1){
            break;
        }
        while(1){
            choice = mailOptions();
            if(choice<1 || choice>3){
                printf("Incorrect option!!\n");
                getchar();
                continue;
            }
            send(sockfd, &choice, sizeof(choice),0);
            switch(choice){
                case 1: if(composeMail(sockfd, creds.username)==0){
                            printf("The specified email address doesn't exist.\n");
                            getchar();
                        }
                        break;
                case 2: viewInbox(sockfd);
                        break;
                case 3: 
                        break;
            }
            if(choice==3){
                break;
            }
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
    offset = recv(sockfd,bfr,size,0) ;
    if(offset<=0){
        break;
    }
    total+=offset;
    bfr+=offset;
  }
  memmove(dest,buffer,total);
  free(buffer);
}

int mainmenu(){
    int choice;
    system("clear");
    printf("MAIN MENU\n");
    printf("1. Login\n");
    printf("2. Create an account\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);
    getchar();
    return choice;
}

Creds account(int sockfd){
    Creds creds;
    char *password;
    system("clear");
    printf("Enter username: ");
    scanf("%s",creds.username);
    password = getpass("Enter password: ");
    strcpy(creds.password,password);
    getchar();
    return creds;
}

int mailOptions(){
    int choice;
    system("clear");
    printf("1. Compose\n");
    printf("2. Inbox\n");
    printf("3. Log out\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);
    return choice;
}

int composeMail(int sockfd, char *from){
    int i;
    char c;
    system("clear");
    Mail mail;
    strcpy(mail.from, from);
    printf("From: %s\n",from);
    printf("To: ");
    scanf("%s",mail.to);
    getchar();
    printf("Subject: ");
    fgets(mail.subject,sizeof(mail.subject),stdin);
    mail.subject[strlen(mail.subject)-1] = '\0';
    printf("Body:\n");
    fgets(mail.body,sizeof(mail.body),stdin);
    mail.body[strlen(mail.body)-1] = '\0';
    send(sockfd, &mail, sizeof(mail),0);
    recvData(sockfd, response, sizeof(response));
    if(strcmp(response,"error")==0){
        return 0;
    }
    return 1;
}

void viewInbox(int sockfd){
    Inbox inbox;
    recvData(sockfd, &inbox, sizeof(Inbox));
    getchar();
    int i=0;
    if(inbox.mailCount==0){
        printf("Inbox is empty!!\n");
        getchar();
        return ;
    }
    while(i < inbox.mailCount){
        system("clear");
        Mail mail = inbox.mails[i++];
        printf("FROM: %s\n",mail.from);
        printf("TO: %s\n",mail.to);
        printf("SUBJECT: %s\n",mail.subject);
        printf("BODY: \n%s\n",mail.body);
        printf("\nPRESS ENTER TO VIEW NEXT MAIL\n");
        getchar();
    }
}
