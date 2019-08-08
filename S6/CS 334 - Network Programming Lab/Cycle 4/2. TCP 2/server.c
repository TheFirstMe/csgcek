#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


#define PORT 8887

int digisum(char *number){
	if(*number)
		return ((*number)-'0') + digisum(number+1);
	else
		return 0;
}

int main(){
	int sockfd, newsockfd, clilen;
	char buffer[256]={0};
	//for storing addresses of server and client
	struct sockaddr_in serv_addr,cli_addr;
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
	//bind the socket to ip and port
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		//binding failed
		printf("Binding failed!!");
		return -1;
	}
	//listening for connection
	if(listen(sockfd,1)){
		//listening failed
		printf("Failed to listen!!");
		return -1;
	}
	//length of client address
	clilen = sizeof(cli_addr);
	//accept a connection from client
	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr,&clilen);
	
	if(newsockfd < 0){
		//accepting connection failed
		printf("Failed to accept!!");
		return -1;
	}
	//receive number from client
	recv(newsockfd,buffer,255,0);
	//find sum of digits
	int n = digisum(buffer);
	//clear buffer
	bzero(buffer,256);
	//print integer sum to buffer
	sprintf(buffer,"%d",n);
	//send sum of digits to back to client
	send(newsockfd,buffer,strlen(buffer)+1,0);
	//close socket
	close(sockfd);
	return 0;
}