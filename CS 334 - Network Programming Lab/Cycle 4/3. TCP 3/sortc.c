#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


#define PORT 8887

//read elements to array
void array(int a[],int n){
	int i;
	printf("Enter array elements: ");
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
	}
}
//print elements of array
void print(int a[],int n){
	int i;
	printf("\nSorted elements: ");
	for(i=0;i<n;i++){
		printf("%d ",a[i]);
	}
}

int main(){
	int sockfd, newsockfd, clilen;
	int buffer[256];
	int n;
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
	printf("\nEnter number of elements in array: ");
	scanf("%d",&n);
	//send number of elements in a array to server
	send(sockfd,&n,sizeof(int),0);
	//read n elements to array
	array(buffer,n);
	//send the array to server
	send(sockfd,buffer,n*sizeof(int),0);
	//clear buffer
	bzero(buffer,256);
	//receive sorted array from server
	recv(sockfd,buffer,n*sizeof(int),0);
	//print the array
	print(buffer,n);
	printf("\n");
	close(sockfd);
	return 0;
}