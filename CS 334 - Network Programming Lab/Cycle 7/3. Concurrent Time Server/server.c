#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>

#define PORT 8889
// int main(){
//     time_t t;
//     time(&t);
//     printf("%s",ctime(&t));
//     return 0;
// }
void error(char *msg){
    perror(msg);
    exit(0);
}

void *connection_handler(void *);

int main(){
    int sockfd,newsockfd,clilen;
    struct sockaddr_in serv_addr,cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
		error("Failed to open socket!!");
    }
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        error("setsockopt(SO_REUSEADDR) failed");
    memset(&serv_addr,0,sizeof(serv_addr));
    memset(&cli_addr,0,sizeof(cli_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error("Error binding!!");
    }
    listen(sockfd,3);
    clilen = sizeof(cli_addr);
    int *sockptr;
    while(newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)){
        pthread_t sniffer_thread;
        if(pthread_create(&sniffer_thread, NULL, connection_handler, &newsockfd) < 0){
            error("cannot create thread");
        }
    }
    if(newsockfd<0){
        error("accept failed");
    }
    return 0;
}

void *connection_handler(void *socket_descriptor){
    int sockfd = *(int *) socket_descriptor;
    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    time_t t;
    time(&t);
    strcpy(buffer,ctime(&t));
    if(send(sockfd, buffer, sizeof(buffer),0) < 0){
        error("cannot send");
    }
    close(sockfd);
}