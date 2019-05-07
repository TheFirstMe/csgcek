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
	the length of server address*/
	int sockfd,len;
	char buffer[256]={0};
	//for server address
	struct sockaddr_in serv_addr;
	//creating a new socket
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
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
	//length of server address
	len = sizeof(serv_addr);
	printf("Enter a message: ");
	fgets(buffer,255,stdin);
	//send message to client
	sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*) &serv_addr,len);
	//clear buffer
	bzero(buffer,255);
	//receive message from server
	recvfrom(sockfd,buffer,255,0,(struct sockaddr*) &serv_addr,&len);
	printf("\nMessage from server: %s ",buffer);
	//close socket
	close(sockfd);
	return 0;
}
