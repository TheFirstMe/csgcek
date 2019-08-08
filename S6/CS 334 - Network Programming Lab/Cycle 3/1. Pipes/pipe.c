#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(){
	char str1[25];
	char str2[25];
	int fd1[2];						//file descriptor for pipe 1
	int fd2[2];						//file descriptor for pipe 2
	pid_t pid;
	printf("Now running from Process P1(pid=%d)....\n\n",getpid());
	printf("\nEnter string: ");
	scanf("%s",str1);
	if(pipe(fd1)==-1){				//creating pipe 1 and checking for failure
		fprintf(stderr,"pipe failed");
		return -1;
	}
	if(pipe(fd2)==-1){				//creating pipe 2 and checking for failure
		fprintf(stderr,"pipe failed");
		return -1;
	}
	pid = fork();					//creating child process
	if(pid<0){						//forking failed
		fprintf(stderr,"fork failed");
		return -1;
	}
	if(pid>0){						//parent process
		close(fd1[0]);				//close read end as it is not used
		write(fd1[1],str1,strlen(str1)+1); //write string to pipe 1
		close(fd1[1]);				//close write end
		wait(NULL);					//wait till child reverses string
		printf("Checking for palindrome from Process P1(pid=%d)....\n\n",getpid());
		close(fd2[1]);				//close write end of pipe 2
		read(fd2[0],str2,25);		//read reversed string from pipe 2 to str2
		close(fd2[0]);				//close read end of pipe 2
		if(strcmp(str1,str2)==0){	//compare input string and its reverse
			printf("\n%s is a palindrome!!\n",str1);
		}
		else{
			printf("\n%s is not a palindrome!!\n",str1);
		}
	}
	else{
		printf("Reversing the string from Process P2(pid=%d)....\n\n",getpid());
		close(fd1[1]);				//close write end of pipe 1
		read(fd1[0],str2,25);		//read input string from pipe 1
		close(fd1[0]);				//close read end of pipe 2
		int i,j=strlen(str2);
		for(i=0;i<j/2;i++){			//reversing string
			str2[i] = str2[j-i-1]; 
		}
		close(fd2[0]);				//close read end of pipe 2
		write(fd2[1],str2,j+1);		//write reversed string to pipe 2
		close(fd2[1]);				//close write end of pipe 2
		exit(0);					//exit from child
	}
	return 0;
}
