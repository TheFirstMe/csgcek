#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

void main(){
    int p,pid,ppid,i;
    pid = getpid();             //For output verification
    printf("Main process id: %d\n",pid);
    
    for(i=0;i<2;i++){           //Calls fork() 2 times to create 3 child processes.
        p = fork();             //Create a new child process
        if(p==0){               //Child process
            pid = getpid();     //Process id
            ppid = getppid();   //Parent Process id
            printf("Child process id: %d, Parent process id: %d\n",pid,ppid);
        }
        else if(p>0){           //Parent process
            wait(NULL);         //To make sure parent doesn't exit before call of getppid()
        }
        else{                   //Error creating child process
            printf("\nError creating process!!");
        }
    }
}