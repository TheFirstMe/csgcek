#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define PORT 8887

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[256];
    char test[256];
    FILE *fp;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    int a;
    a = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(a<0)
        error("Connect error");
    
    system("clear");
    printf("\nEnter file name: ");
    scanf("%s",buffer);
    strcpy(test,buffer);
    a=send(sockfd,buffer,strlen(buffer)+1,0);
    if(a<0){
        error("Send error");
    }
    a=recv(sockfd,buffer,sizeof(buffer),0);
    if(a<0){
        error("Receive error");
    }
    if(strcmp(buffer,"error")==0){
   		printf("File doesn't exist!!");
        fflush(stdout);
   		fclose(fp);
   		return 0;
    }
    int size = atoi(buffer);
    float total=0;
    fp = fopen(test,"wb");
    bzero(buffer,256);
    while((a=recv(sockfd,buffer,sizeof(buffer),0)) != 0){
        total+=a;
        printf("\rReceiving %s from server: %.2f%%",test,(total/size)*100);
        if(a<0){
            error("Receive error");
        }
        fwrite(buffer,1,a,fp);
        bzero(buffer,256);
    }
    printf("\n");
    printf("\n%s received successfully\n",test);
    fflush(stdout);
    fclose(fp);
    return 0;
}
