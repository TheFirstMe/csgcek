#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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
    int sockfd;
    //for storing address of server
    struct sockaddr_in serv_addr;
    char buffer[256];
    
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
    //connecting to server
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        //connection failed
        printf("Failed to connect!!");
        return -1;
    }

    printf("Enter a message to be sent to the server: ");
    scanf("%s",buffer);
    //send buffer content to server
    if(send(sockfd,buffer,strlen(buffer),0) < 0){
        printf("\nError sending message to server!!");
        return -1;
    }
    loading("Sending message to server");
    printf("Message sent to server successfully!!\n");
    //clear buffer
    bzero(buffer,256);
    //receive acknowledgment from server
    if(recv(sockfd,buffer,255,0) < 0){
        printf("\nError receiving message from server!!");
        return -1;
    }
    loading("Incoming message from server");
    printf("Acknowledgment received from server: %s\n",buffer);
    return 0;
}