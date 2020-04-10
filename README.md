# SoalShiftSISOP20_modul3_A05
**Intan Kusuma R  (05111840000020)**

**Rafid Ferdianto (05111840000032)**

## Soal 1
Source code: 
### Penjelasan soal dan penyelesaian
**Kendala**

**Screenshot Hasil**

## Soal 2
Source code: 
### Penjelasan soal dan penyelesaian
**Kendala**

**Screenshot Hasil**

## Soal 3
Source code: 
### Penjelasan soal dan penyelesaian
Buat sebuah program C untuk mengkategorikan file. Program ini akan
memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah
sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working
directory ketika program kategori tersebut dijalankan.

Ada 3 opsi untuk menjalankan program:
* opsi -f: digunakan untuk mengkategorikan file untuk file-file tertentu sebagai argumen.
* opsi * : digunakan untuk mengkategorikan file untuk seluruh file dalam current working directory saat program dijalankan.
* opsi -d: digunakan untuk mengkategorikan file dalam directory tertentu sebagai argumen.

**Kendala**

**Screenshot Hasil**

## Soal 4
Source code:

* [soal4a.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal4/soal4a.c)
* [soal4b.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal4/soal4b.c)
* [soal4c.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal4/soal4c.c)
### Penjelasan soal dan penyelesaian
a. Buat program C dengan nama "4a.c", yang berisi program untuk
melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2 , dan
matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks
nantinya akan berisi angka 1-20 ( tidak perlu dibuat filter angka). Tampilkan matriks hasil perkalian tadi ke layar.

* Deklarasi matriks m1, m2, dan m sebagai matriks hasil perkalian
```c
int m1[B][BK] = { {1,2}, {2,3}, {3,2}, {2,1}},
m2[BK][K] = { {1,2,1,2,1}, {2,1,2,1,2} },	
m[B*K]; 
```
* Membuat kode unik untuk menyambungkan ke program 4b.c dan array `res` untuk menampung nilai matrix hasil perkalian ke program 4b.c 
```c
key_t key=1412;
	
int shmid=shmget(key, sizeof(int)*4*5, IPC_CREAT | 0666 );
int *res=(int *)shmat(shmid, NULL, 0);
```
* Print matriks 1 dan 2
```c
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
```
* Deklarasi array thread dengan ukuran 20
```c++
pthread_t *tid=(pthread_t*)malloc((20)*sizeof(pthread_t));
```
* Menyimpan elemen dari baris dan kolom ke `arg`
```c++
arg=(int*)malloc((20)*sizeof(int));
arg[0]=BK;

for(k=0; k<BK; k++){
	arg[k+1]=m1[i][k];
}
for(k=0; k<BK; k++){
	arg[k+BK+1]=m2[k][j];
}
```
* Buat thread
```c++
err = pthread_create(&tid[count++],NULL,mul,(void*) arg);
```
* Join thread dan ambil return value
* Print hasil perkalian matriks 
```c
for(i=0; i < 20; i++){
	void*k;
	pthread_join(tid[i],&k); 
	int* p= (int* )k;
	printf("%d ",*p);
	if((i+1)%K==0)
	printf("\n");
	m[i]=*p;
}
```
* Masukkan hasil perkalian matriks ke `res`.
```c++
for(i=0; i < B*K; i++){
	res[i]=m[i];
}
```
* Fungsi `mul`
* Loop untuk melakukan perkalian
* Return value di pass sebagai pointer
```c++
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
```
b. Buat program C dengan nama "4b.c". Program ini akan
mengambil variabel hasil perkalian matriks dari program "4a.c" (program
sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
( Catatan! : gunakan shared memory). Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai penjumlahan dari 1 hingga n, dan tampilkan hasilnya ke layar dengan format seperti matriks.
* Membuat kode unik untuk menyambungkan dan array `res` untuk menampung nilai matrix hasil perkalian
```c
key_t key=1412;
int shmid=shmget(key,sizeof(int)*4*5, IPC_CREAT | 0666);
int* res=(int*)shmat(shmid,NULL,0);
```
* Masukkan `res` ke array `hasil`
```c
for(i=0; i<20; i++)
	hasil[i]=res[i];
```
* Print matriks hasil perkalian
```c++
printf("Matriks hasil perkalian\n");
for(i=0; i<20; i++){
	printf("%d ", hasil[i]);
	if((i+1)%5==0)
		printf("\n");
}
```
* Buat thread
```c++
int *arg = (int*) malloc(sizeof(int));
arg=&hasil[i];
err = pthread_create(&tid[j++],NULL,fungsi,(void*) arg);
```
* Join thread dan ambil return value
* Print hasil penjumlahan
```c
printf("Hasil penjumlahan\n");
for(i=0; i<20; i++){
	void* k;
	pthread_join(tid[i], &k);
	int* p=(int*)k;
	printf("%d ", *p);
	if((i+1)%5==0)
		printf("\n");
}
```
* Fungsi `fungsi` untuk melakukan penjumlahan dari 1 sampai n
* `angka` digunakan untuk menampung nilai array
* Loop untuk melakukan penjumlahan
* Return value di pass sebagai pointer
```c
void* fungsi(void *arg)
{
	int angka=*(int*)arg;
	int factorial=0;
	int i,j;
	for(i=0; i<=angka; i++)
	{   
		factorial+=i;
	}
	int *p=(int*)malloc(sizeof(int));
	*p=factorial;
 
	pthread_exit(p);
}
```
c. Buat program C dengan nama "4c.c". Program ini tidak
memiliki hubungan terhadap program yang lalu. Pada program ini, diminta mengetahui jumlah file dan
folder di direktori saat ini dengan command `ls | wc -l`.
* `pipe1[0]` adalah file descriptor untuk read end dan `pipe1[1]` adalah file descriptor untuk write end
```c
int pipe1[2];
```
* Jika pipe gagal
```c
if (pipe(pipe1) == -1)
  exit(1);
```
* Child process
* Membuat stdout dan close file descriptor
* Eksekusi `ls`
```c
if ((fork()) == 0) 
{
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  char *argv1[] = {"ls", NULL};
  execv("/bin/ls", argv1);
}
```
* Parent process
* Read hasil `ls` dan close file descriptor
* Eksekusi `wc -l`
```c
else
{
// input from pipe1
dup2(pipe1[0], 0);
// close fds
close(pipe1[0]);
close(pipe1[1]);
// exec
char *argv1[] = {"wc", "-l", NULL};
execv("/usr/bin/wc", argv1);
}
```
**Kendala**

**Screenshot Hasil**
