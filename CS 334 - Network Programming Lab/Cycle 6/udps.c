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
	/*for getting file descriptor for socket and storing
	the length of client address*/
	int sockfd, clilen;
	char buffer[256]={0};
	//for server and client addresses
	struct sockaddr_in serv_addr,cli_addr;
	//creating a new socket
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	//check if socket creation is successful
	if(sockfd < 0){
		//error opening socket
		printf("Failed to open socket!!");
		return -1;
	}
	//specifies the addressing family used
	serv_addr.sin_family = AF_INET;
	//ip address of server
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//port
	serv_addr.sin_port = htons(PORT);
	//bind the socket to specified ip,port and addressing family
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		//binding failed
		printf("Binding failed!!");
		return -1;
	}
	//store length of client address
	clilen = sizeof(serv_addr);
	while(1){
		//receive message from any client to buffer
		recvfrom(sockfd,buffer,255,0,(struct sockaddr*) &cli_addr,&clilen);
		if(strcmp(buffer,"exit\n")==0){
			break;
		}
		printf("\nMessage from client: %s\n",buffer);
		//clear buffer	
		bzero(buffer,255);
		printf("\nEnter message from server: "); 
		fgets(buffer,255,stdin);
		//send message to the client
		sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*) &cli_addr,clilen);
		if(strcmp(buffer,"exit\n")==0){
			break;
		}
	}
	//close socket
	close(sockfd);
	return 0;
}
