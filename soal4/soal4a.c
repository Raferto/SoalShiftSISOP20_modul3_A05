#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>

#define B 4
#define BK 2
#define K 5

typedef struct pass{
	int i, j;
} pass;

void* mul(void* struk);

int m1[B][BK] = { {1,2}, {2,3}, {3,2}, {2,1}},	// matriks 
	m2[BK][K] = { {1,2,1,2,1}, {2,1,2,1,2} },	// matriks 
    m[B][K]; // matriks hasil

int main(){
	
	key_t key=1412;
	
	int shmid=shmget(key, sizeof(int)*4*5, IPC_CREAT | 0666 );
	int *res=(int *)shmat(shmid, NULL, 0);
	
    int i,j;
    printf("Matriks 1 %dx%d:\n", B, BK);
	for(i=0; i < B; i++){
		for(j=0; j < BK; j++)
			printf("%d ",m1[i][j] );
		printf("\n");
	}

	printf("\nMatriks 2 %dx%d:\n", BK, K);
	for(i=0; i < BK; i++){
		for(j=0; j < K; j++)
			printf("%d ",m2[i][j] );
		printf("\n");
	}

	int err;
	for(i=0; i < B; i++){
        for(j=0; j < K; j++){
	        pass *arg = (pass*) malloc(sizeof(pass));
            arg->i=i;
            arg->j=j;

            pthread_t tid;
			err = pthread_create(&tid,NULL,mul,(void*) arg);
			if(err!=0)
				printf("\n can't create thread : [%s]",strerror(err));
			// else
				// printf("\n create thread success\n");

			pthread_join(tid,NULL); 
		}
    }

	printf("\nHasil Kali Matriks %dx%d:\n", B, K);
	for(i=0; i < B; i++){
		for(j=0; j < K; j++){
            res[i*5+j]=m[i][j];
			printf("%d ",m[i][j] );
        }
		printf("\n");
	}

	shmdt(res);
	exit(0);
	return 0;
}

void* mul(void* struk){
	pass *arg = (pass*)struk;
    
	int x,temp=0;
	pthread_t id=pthread_self();

	for(x=0; x < B; x++){
        temp+=m1[arg->i][x] * m2[x][arg->j];
    }
	m[arg->i][arg->j]=temp;
	return NULL;
}