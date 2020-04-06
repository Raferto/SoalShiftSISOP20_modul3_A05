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
* Membuat struct pass, i adalah baris dan j adalah kolom
```c
typedef struct pass{
	int i, j;
} pass;
```
* Deklarasi matriks m1, m2, dan m sebagai matriks hasil perkalian
```c
int m1[B][BK] = { {1,2}, {2,3}, {3,2}, {2,1}},
m2[BK][K] = { {1,2,1,2,1}, {2,1,2,1,2} },	
m[B][K]; 
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
* Menetapkan baris dan kolom pada setiap thread
* Pointer `tid` digunakan untuk menunjukkan alamat memori dengan thread ID dari thread baru.
* Nilai `attr` di-set `NULL` ketika thread menggunakan atribut default.
* Parameter ketiga adalah fungsi `mul` untuk menghitung perkalian dua matriks. 
* Pointer `arg` digunakan untuk memberikan sebuah argumen ke fungsi `mul`
* Menunggu thread sebelumnya untuk selesai dengan `pthread_join(tid,NULL)`
```c
for(i=0; i < B; i++){
  for(j=0; j < K; j++){
    pass *arg = (pass*) malloc(sizeof(pass));
    arg->i=i;
    arg->j=j;
    pthread_t tid;
    err = pthread_create(&tid,NULL,mul,(void*) arg);
    if(err!=0)
      printf("\n can't create thread : [%s]",strerror(err));
    pthread_join(tid,NULL); 
  }
}
```
* Print hasil perkalian matriks dan mengcopy hasil perkalian matriks ke `res`.
```c
printf("\nHasil Kali Matriks %dx%d:\n", B, K);
for(i=0; i < B; i++){
  for(j=0; j < K; j++){
    res[i*5+j]=m[i][j];
    printf("%d ",m[i][j] );
  }
  printf("\n");
}
```
* Fungsi `mul` yang digunakan untuk melakukan perkalian matriks
* `pass *arg` digunakan untuk menampung nilai tiap elemen matriks
* Loop untuk melakukan perkalian tiap elemen matriks dan hasilnya di letakkan di matriks m
```c
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
```
b. Buat program C dengan nama "4b.c". Program ini akan
mengambil variabel hasil perkalian matriks dari program "4a.c" (program
sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
( Catatan! : gunakan shared memory). Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai penjumlahan dari 1 hingga n, dan tampilkan hasilnya ke layar dengan format seperti matriks.
* Membuat struct `pass`
```c
typedef struct pass{
  int i;
} pass;
```
* Membuat kode unik untuk menyambungkan dan array `res` untuk menampung nilai matrix hasil perkalian
```c
key_t key=1412;
int shmid=shmget(key,sizeof(int)*4*5, IPC_CREAT | 0666);
int* res=(int*)shmat(shmid,NULL,0);
```
* Print matriks hasil perkalian dan copy array `res` ke array `number`
```c
printf("Matriks hasil perkalian\n");
for(i=0; i<4; i++){
  for(j=0; j<5; j++){
    printf("%d ", res[i*5+j]);
    number[i*5+j]=res[i*5+j];
  }
  printf("\n");
}
printf("\n");
```
* Pointer `tid` digunakan untuk menunjukkan alamat memori dengan thread ID dari thread baru.
* Nilai `attr` di-set `NULL` ketika thread menggunakan atribut default.
* Parameter ketiga adalah fungsi `fungsi` untuk menghitung penjumlahan dari 1 sampai n. 
* Pointer `arg` digunakan untuk memberikan sebuah argumen ke fungsi `fungsi`
* Menunggu thread sebelumnya untuk selesai dengan `pthread_join(tid,NULL)`
```c
for (i=0;i<20;i++)
{
  pass *arg = (pass*) malloc(sizeof(pass));
  arg->i=i;
  pthread_t tid;
  err = pthread_create(&tid,NULL,fungsi,(void*) arg);
  if(err!=0)
    printf("\n can't create thread : [%s]",strerror(err));
  pthread_join(tid,NULL);
}
```
* Print hasil penjumlahan
```c
printf("Hasil penjumlahan\n");
for (i=0;i<4;i++){
  for(j=0;j<5;j++)
  {
    printf("%d ", matrix[i*5+j]);
  }
  printf("\n");
}
```
* Fungsi `fungsi` untuk melakukan penjumlahan dari 1 sampai n
* `pass *angka` digunakan untuk menampung nilai tiap elemen array
* Loop untuk melakukan penjumlahan tiap elemen dan hasilnya di letakkan di array `matrix`
```c
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
