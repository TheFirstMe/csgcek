#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


#define PORT 8887


int main(){
	int sockfd, newsockfd, clilen;
	char buffer[256]={0};
	//for storing address and port of server
	struct sockaddr_in serv_addr;
	//creating a new socket
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	if(sockfd < 0){
		//failed to create socket
		printf("Failed to open socket!!");
		return -1;
	}
	
	//specifies the addressing family used
	serv_addr.sin_family = AF_INET;
	//ip address of server
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//port
	serv_addr.sin_port = htons(PORT);
	
	//establish a connection to server
	if(connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
		//failed to make connection
		printf("Failed to connect!!");
		return -1;
	}
	
	printf("Enter a number: ");
	scanf("%s",buffer);
	//send number to server
	send(sockfd,buffer,strlen(buffer)+1,0);
	bzero(buffer,256);
	//receive sum of digits from server
	recv(sockfd,buffer,255,0);
	printf("\nSum of digits: %s\n",buffer);
	//close socket
	close(sockfd);
	return 0;
}