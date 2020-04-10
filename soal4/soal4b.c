#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>

void* fungsi(void *arg)
{
	int angka=*(int*)arg;
    int factorial=0;
    int i,j,c;
    for(i=0; i<=angka; i++)
	{   
	    factorial+=i;
	}
    int *p=(int*)malloc(sizeof(int));
	*p=factorial;
    
	pthread_exit(p);
}

int main(void)
{
	key_t key=1412;
	int shmid=shmget(key,sizeof(int), IPC_CREAT | 0666);
	int* res=(int*)shmat(shmid,NULL,0);
	
	int hasil[20];
	int i;

	for(i=0; i<20; i++)
		hasil[i]=res[i];

    printf("Matriks hasil perkalian\n");
	for(i=0; i<20; i++){
		printf("%d ", hasil[i]);
		if((i+1)%5==0)
		printf("\n");
	}
	
	pthread_t tid[20];
	int j=0, x;

    int err;
    for (i=0;i<20;i++)
    {
    	int *arg = (int*) malloc(sizeof(int));
		arg=&hasil[i];
		err = pthread_create(&tid[j++],NULL,fungsi,(void*) arg);
		if(err!=0)
			printf("\n can't create thread : [%s]",strerror(err));
		// else
			// printf("\n create thread success\n");
    }

    printf("Hasil penjumlahan\n");
	for(i=0; i<20; i++){
		void* k;
		pthread_join(tid[i], &k);
		int* p=(int*)k;
		printf("%d ", *p);
		if((i+1)%5==0)
			printf("\n");
	}

    return 0;
}

