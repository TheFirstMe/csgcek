#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//port number to be used
#define PORT 8889

void loading(char *message){
    int i,j;
    printf ( "\n");
    for (i=0; i<2; i++) {
        for (j=0; j<4; j++) {
            printf("\r%s%.*s   \b\b\b",message, j, "...");
            fflush(stdout);//force printing as no newline in output
            usleep(1000000);
        }
    }
    printf("\n");
}

int main()
{
    int sockfd, newsockfd, clilen;
    char buffer[256];
    //for storing addresses of server and client
    struct sockaddr_in serv_addr, cli_addr;

    //creating a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        //failed to create socket
        printf("Failed to open socket!!");
        return -1;
    }

    //specifies the addressing family used
    serv_addr.sin_family = AF_INET;
    //ip address of server
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);
    //port
    serv_addr.sin_port = htons(PORT);

    //bind the socket to ip and port
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        //binding failed
//        printf("Failed to bind!!");
	perror("Error: ");
	exit(0);
        return -1;
    }
    
    //listen for connections
    if(listen(sockfd,1)){
        printf("Failed to listen!!");
        return -1;
    }

    //length of client address
    clilen = sizeof(cli_addr);
    //accept a connection from client
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
    if(newsockfd < 0) {
        //accepting connection failed
        printf("Failed to accept!!");
        return -1;
    }
    //receive message from client to buffer
    if(recv(newsockfd,buffer,255,0) < 0){
        printf("\nError receiving message from client!!");
        return -1;
    }
    loading("Incoming message from client");
    printf("Message received from client: %s\n",buffer);
    printf("\nEnter message to be sent to the client: ");
    //clear buffer
    bzero(buffer,256);
    scanf("%s",buffer);
    //send buffer content to client as acknowledgment
    if(send(newsockfd,buffer,sizeof(buffer),0) < 0){
        printf("\nError sending acknowledgment to client!!");
        return -1;
    }
    loading("Sending message to client");
    printf("Acknowledgment sent to client successfully!!\n");
    return 0; 
}