#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define PORT 8887

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(){
    int sockfd;
    int newsockfd;
    struct sockaddr_in serv_addr,cli_addr;
    char buffer[256];
    char test[256];
    int clilen;

    FILE *fp;
    struct stat st;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        error("setsockopt(SO_REUSEADDR) failed");

    memset(&serv_addr,0,sizeof(serv_addr));
    memset(&cli_addr,0,sizeof(cli_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    listen(sockfd,5);
    int i=0,pid,b,size;
    while(1){
        bzero(buffer,256);
        bzero(test,256);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
        if(newsockfd<0){
            error("accept error");
        }
        system("clear");
        printf("Connected to client \n");
		char *message;
        b=recv(newsockfd,buffer,sizeof(buffer),0);
        if(b<0){
            error("receive error");
        }
        fp = fopen(buffer,"rb");
        if(fp==NULL){
            message = "error";
            b = send(newsockfd,message,strlen(message)+1,0);
        	if(b<0){
                error("send error");
            }
            fclose(fp);
            close(newsockfd);
            continue;
        }
        else{
            int fd = open(buffer,O_RDONLY);
            int errno = 0; 
            if(fstat(fd, &st)){ 
                printf("\nfstat error: [%s]\n",strerror(errno)); 
                return -1; 
            } 
            size = st.st_size;
            char sizestr[100];
            sprintf(sizestr,"%d",size);
            b = send(newsockfd,sizestr,strlen(sizestr)+1,0);
        	if(b<0){
                error("send error");
            }
        }
        printf("\n");
        bzero(test,256);
        float total = 0;
        int a;
        while((a=fread(test,1,sizeof(test),fp)) != 0){
            total+=a;
            printf("\rSending %s to client: %.2f%%",buffer,(total/size)*100);
            fflush(stdout);
            b = send(newsockfd,test,a,0);
            if(b<0){
                error("send error");
            }
        }
        printf("\n");
        fclose(fp);
        printf("\n%s sent to client successfully\n",buffer);
        fflush(stdout);
        close(newsockfd);
    }    
    close(sockfd);
    return 0;
}
