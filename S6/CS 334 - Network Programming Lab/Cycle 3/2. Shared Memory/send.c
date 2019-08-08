#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

//a structure to hold array,size,max and min elements
typedef struct Array{                       
    int data[10];
    int n;
    int max;
    int min;
}Array;

//function to read elements to array
void readArray(int a[],int n){              
    int i;
    printf("Enter elements in the array: ");
    for(i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
}

int main() 
{ 
    pid_t pid;
    printf("Now running from Process P1(pid=%d)....\n\n",getpid());
    // ftok to generate unique key 
    key_t key = ftok("guhan",64); 
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,sizeof(Array),0666|IPC_CREAT); 
    // shmat to attach to shared memory 
    Array *array = (Array*) shmat(shmid,0,0); 
    printf("Enter number of elements in array: ");
    scanf("%d",&array->n);
    readArray(array->data,array->n);    
    printf("Process P1 has passed array to shared memory.\n");
    //creating process P2
    pid = fork();
    if(pid<0){                                      
        //process creation failed
        fprintf(stderr,"Error creating process!");
        shmctl(shmid,IPC_RMID,NULL);
        return -1;
    }
    else if(pid>0){                                 
        //in process P1
        //wait till process P2 exits
        wait(NULL);                                 
        int max,min;
        printf("\nNow running from Process P1(pid=%d)....\n\n",getpid());
        max = array->max;
        min = array->min;
	    //detach from shared memory 
	    shmdt(array);
        //destroy shared memory
        shmctl(shmid,IPC_RMID,NULL);
        printf("Largest element: %d, Smallest element: %d",max,min);
    }
    else{                                           
        //in process P2
	    printf("\nNow running from Process P2(pid=%d)....\n\n",getpid());
	    printf("Process P2 has accessed the array from the shared memory.\n");
        int max,min,i;
        //assuming first element of array as smallest and largest
        max=min=array->data[0];
        for(i=1;i<array->n;i++){
            int val = array->data[i];
            if(val<min)
                min=val;
            if(val>max)
                max=val;
        }
        array->max=max;
        array->min=min;
        //exit from child
        exit(0);
    }
    return 0; 
}
