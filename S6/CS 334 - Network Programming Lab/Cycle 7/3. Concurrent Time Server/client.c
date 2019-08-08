#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#define PORT 8889

void error(char *msg){
    perror(msg);
    exit(0);
}

int main(){
    int sockfd;
    char buffer[256];
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error("Error creating socket");
    memset(&serv_addr,0,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    int servsize = sizeof(serv_addr);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, servsize) < 0){
        error("error connecting");
    }

    memset(buffer,0,sizeof(buffer));
    recv(sockfd,buffer,sizeof(buffer),0);
    printf("\nServer time: %s",buffer);
    // close(sockfd);
    return 0;
}