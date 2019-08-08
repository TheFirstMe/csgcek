#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define PORT 8887

//a function to check if a character is present in the string
int inString(char *str, char c){
    int i, len=strlen(str);
    for(i=0;i<len;i++){
        if(str[i]==c && c!=' ')
            return 1;
    }
    return 0;
}

//a function that finds the vowels, consonants and no. of words
void vowel(char *str, char *v,char *c,int *wc){
    int len = strlen(str),i;
    int wcnt=0;
    int j=0,k=0;
    for(i=0;i<len;i++){
        //skip duplicate characters
        if(inString(v,str[i]) || inString(c,str[i])){
            continue;
        }
        switch(tolower(str[i])){
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u': //vowel has occurred
                      sprintf(v+j,"%c ",str[i]);
                      j+=2;
                      break;
            case ' ': //space occurred => increment word count
                      wcnt++;
                      while(str[i]==' '){
                          //skip unnecessary spaces
                          i++;
                      }
                      i--;
                      break;
            default: 
                      if(isalpha(str[i])){
                         //consonant has occurred
                         sprintf(c+k,"%c ",str[i]);
                         k+=2;
                      }
                      break;
        }
    }
    //no. of words = no. of spaces + 1
    *wc = wcnt+1;
}

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
	//receive message from any client to buffer
	recvfrom(sockfd,buffer,255,0,(struct sockaddr*) &cli_addr,&clilen);
    printf("\nString from client: %s",buffer);
    int wcnt;
    char vowels[256]={0};
    char consonants[256]={0};
    vowel(buffer,vowels,consonants,&wcnt);
    //clear buffer
    bzero(buffer,255);
    //print result to buffer
    sprintf(buffer,"Number of words = %d\nVowels: %s\nConsonants: %s\n",wcnt,vowels,consonants);
    //send result to client
    sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*) &cli_addr,clilen);
    close(sockfd);
    return 0;
}