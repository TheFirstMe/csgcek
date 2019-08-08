#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


#define PORT 8887

//function to perform bubble sort
void sort(int a[],int n){
	int i,j,swapped;
	for(i=0;i<n-1;i++){
		swapped = 0;
		for(j=0;j<n-i-1;j++){
			if(a[j]>a[j+1]){
				int temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
				swapped=1;
			}
		}
		//if swapped is not set, the array is sorted
		if(!swapped)
			break;
	}
}



int main(){
	int sockfd, newsockfd, clilen;
	int buffer[256]={0};
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
	//listen for connections
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
	int n;
	//receive size of array from client
	recv(newsockfd,&n,sizeof(int),0);
	//receive n elements from client
	recv(newsockfd,buffer,n*sizeof(int),0);
	//perform bubble sort
	sort(buffer,n);
	//send sorted array back to client
	send(newsockfd,buffer,n*sizeof(int),0);
	//close socket
	close(sockfd);
	return 0;
}
