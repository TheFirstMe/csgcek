#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<strings.h>

#define PORT 8887

void remwp(char *string){			//function to replace whitespaces by null character
	int i;
	for(i=0;string[i]!='\0';i++){
		if(string[i]==' ')
			string[i] = '\0';
	}
}

void unique(char *newstring, char *oldstring){	//function to remove duplicate words
	//store length of input string before replacing whitespaces with null characters
	int len = strlen(oldstring);
	remwp(oldstring);
	//i is a pointer to each word in the string
	int i;
	for(i=0;*(oldstring+i)!='\0';i+=strlen(oldstring+i)+1){
		//j is another pointer to each word in the string
		int j=0;
		//compare each word in the string with every other word
		for(;;){
			//get pointer to potentially repeated word 
			if(strcmp(oldstring+i,oldstring+j)==0){
				//check if base addresses of words are equal
				if(oldstring+i==oldstring+j){
					//if equal, add the word to result
					strcat(newstring,oldstring+i);
					int temp = strlen(newstring); 
					newstring[temp] = ' ';
					newstring[temp+1] = '\0';
				}
				//or else don't add it
				break;
			}
			//point to next word in the string
			j+=strlen(oldstring+j)+1;
		}
	}
}

int main(){
	int sockfd, newsockfd, clilen,opt=1;
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
	
	//enable reuse of address
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
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
	//receive message from client
	recv(newsockfd,buffer,255,0);
	char result[255]={0};
	//remove repeated words
	unique(result,buffer);
	//send result to client
	send(newsockfd,result,strlen(result)+1,0);
	return 0;
}
