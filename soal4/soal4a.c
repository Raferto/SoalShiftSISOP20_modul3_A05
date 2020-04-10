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

void* mul(void* struk){
	int *arg = (int*)struk;
    int k=0, i=0;

	int x=arg[0];
	for(i=1; i<=x; i++)
		k+=arg[i]*arg[i+x];
	
	int *p=(int*)malloc(sizeof(int));
	*p=k;
	pthread_exit(p);
}

int m1[B][BK] = { {1,2}, {2,3}, {3,2}, {2,1}},	// matriks 
	m2[BK][K] = { {1,2,1,2,1}, {2,1,2,1,2} },	// matriks 
    m[B*K]; // matriks hasil

int main(){
	
	key_t key=1412;
	
	int shmid=shmget(key, sizeof(int)*4*5, IPC_CREAT | 0666 );
	int *res=(int *)shmat(shmid, NULL, 0);
	
    int i,j,k;
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
	pthread_t *tid=(pthread_t*)malloc((20)*sizeof(pthread_t));

	int count=0;
	int* arg=NULL;

	for(i=0; i < B; i++){
        for(j=0; j < K; j++){

			arg=(int*)malloc((20)*sizeof(int));
			arg[0]=BK;

			for	(k=0; k<BK; k++){
				arg[k+1]=m1[i][k];
			}
			for	(k=0; k<BK; k++){
				arg[k+BK+1]=m2[k][j];
			}

				
			err = pthread_create(&tid[count++],NULL,mul,(void*) arg);
			if(err!=0)
				printf("\n can't create thread : [%s]",strerror(err));
			// else
				// printf("\n create thread success\n");
		}
    }

	printf("\nHasil Kali Matriks %dx%d:\n", B, K);
	for(i=0; i < 20; i++){
		void*k;
		pthread_join(tid[i],&k); 
		int* p= (int* )k;
		printf("%d ",*p);
		if((i+1)%K==0)
		printf("\n");
		m[i]=*p;
		
	}


	for(i=0; i < B*K; i++){
            res[i]=m[i];
        
	}

	shmdt(res);
	exit(0);
	return 0;
}

