#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>

typedef struct pass{
	int i;
} pass;

int k=0, input;
int number[21],matrix[21];
int factorial=0,b=0,d=0;

void* fungsi(void *arg);

int main(void)
{
	key_t key=1412;
	int shmid=shmget(key,sizeof(int)*4*5, IPC_CREAT | 0666);
	int* res=(int*)shmat(shmid,NULL,0);
	
	int i, j;
    printf("Matriks hasil perkalian\n");
	for(i=0; i<4; i++){
		for(j=0; j<5; j++){
			printf("%d ", res[i*5+j]);
			number[i*5+j]=res[i*5+j];
		}
		printf("\n");
	}
	printf("\n");

    int err;
    for (i=0;i<20;i++)
    {
    	pass *arg = (pass*) malloc(sizeof(pass));
            arg->i=i;
           

            pthread_t tid;
			err = pthread_create(&tid,NULL,fungsi,(void*) arg);
			if(err!=0)
				printf("\n can't create thread : [%s]",strerror(err));
			// else
				// printf("\n create thread success\n");

			pthread_join(tid,NULL);
    }

    printf("Hasil penjumlahan\n");
    for (i=0;i<4;i++){
        for(j=0;j<5;j++)
        {
            printf("%d ", matrix[i*5+j]);
        }
        printf("\n");
    }
    return 0;
}

void* fungsi(void *arg)
{
	pass* angka=arg;
    pthread_t id=pthread_self();
    int i,j,c;
    for(i=0; i<20; i++){
	    for(j=0;j<=number[b];j++)
	    {   
	        factorial+=j;
	    }
    matrix[d]=factorial;
    factorial=0;
    b+=1;
    d+=1;
	}
    return NULL;
}