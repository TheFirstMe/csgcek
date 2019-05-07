#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

void readArray(int a[],int n){              //function to read array elements
    int i;
    printf("\nEnter array elements: ");
    for(i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
}

void printArray(int a[],int n){             //function to print array elements
    int i;
    for(i=0;i<n;i++){
        printf("%d ",a[i]);
    }
    printf("\n");
}

void sort(int a[],int n){                   //function for performing bubble sort
    int i,j;
    for(i=0;i<n-1;i++){
        int sorted = 1;
        for(j=0;j<n-i-1;j++){
            if(a[j]>a[j+1]){
                a[j]=a[j]+a[j+1]-(a[j+1]=a[j]); //swap
                sorted = 0;
            }
        }
        if(sorted)                          //already sorted
            return ;
    }
}

void main(){
    int p,pid,ppid,a[10],n;
    pid = getpid();                         //for output verification
    printf("Main process id: %d",pid);
    printf("\nEnter the number of elements in the array: ");
    scanf("%d",&n);
    readArray(a,n);
    p = fork();                             //create a new child process
    
    if(p==0){                               //child process
        pid = getpid();                     //process id
        ppid = getppid();                   //parent process id
        printf("\nCurrently in child process (pid=%d, ppid=%d).",pid,ppid);
        printf("\nUnsorted array elements are: ");
        printArray(a,n);
    }
    else if(p>0){                           //parent process
        wait(NULL);                         /*To make sure parent doesn't exit 
                                            before call of getppid() and array
                                            is not sorted before child terminates*/
        sort(a,n);
        pid = getpid();                     //Parent process id
        printf("\nCurrently in parent process (pid=%d).",pid);
        printf("\nSorted array elements are: ");
        printArray(a,n);
    }
    else{                                   //error creating child process
        printf("Error creating process!!");
    }
}