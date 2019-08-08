//use -lpthread during compilation

#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>

int a[5][5],b[5][5],c[5][5],m,n;

void readMatrix(int a[5][5],int m,int n,char *msg){     //function for reading matrix elements
    int i,j;
    printf("\nEnter matrix %s elements: \n",msg);
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            scanf("%d",&a[i][j]);
        }
    }
}

void printMatrix(int a[5][5],int m,int n){              //function for printing matrix elements
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%d \t",a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void *addMatrices(void *arg){                           //thread function for adding 2 matrices
    int i,j;
    i = *((int *)arg);
    for(j=0;j<n;j++){
        c[i][j] = a[i][j] + b[i][j];
    }

}

void main(){
    int i;
    pthread_t p[5];                                     //Thread variables denoting matrix rows
    printf("Enter dimensions of matrices: ");
    scanf("%d%d",&m,&n);
    readMatrix(a,m,n,"A");
    readMatrix(b,m,n,"B");
    for(i=0;i<m;i++){
        pthread_create(&p[i],NULL,addMatrices,&i);      //create new thread
        pthread_join(p[i],NULL);                        //block other threads
    }
    printf("\nSum of matrices: \n");
    printMatrix(c,m,n);
}