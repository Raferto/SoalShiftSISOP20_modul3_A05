# SoalShiftSISOP20_modul3_A05
**Intan Kusuma R  (05111840000020)**

**Rafid Ferdianto (05111840000032)**

## Soal 1
Source code: 
### Penjelasan soal dan penyelesaian
**Kendala**
- tidak begitu memahami soal dan implementasinya

**Screenshot Hasil**

## Soal 2
Source code: [soal2_client.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal2/soal2_client.c)
	     [soal2_server.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal2/soal2_server.c)
### Penjelasan soal dan penyelesaian
**Kendala**
- kesulitan mengimplementasikan soal
**Screenshot Hasil**
![2.1](https://user-images.githubusercontent.com/57932819/79036704-1d7a9c00-7bf5-11ea-8dd8-1362c296f5a3.png)
![2.2](https://user-images.githubusercontent.com/57932819/79036708-24a1aa00-7bf5-11ea-9be7-d672a159a90b.png)
![2.3](https://user-images.githubusercontent.com/57932819/79036719-3edb8800-7bf5-11ea-80a5-3a80c4540bb5.png)
![2.4](https://user-images.githubusercontent.com/57932819/79036726-5155c180-7bf5-11ea-9cd2-eb4ef7ac8015.png)
![2.5](https://user-images.githubusercontent.com/57932819/79036727-53b81b80-7bf5-11ea-9648-999b79ada88e.png)
![2.6](https://user-images.githubusercontent.com/57932819/79036728-561a7580-7bf5-11ea-8246-213af3eb24c5.png)
![2.7](https://user-images.githubusercontent.com/57932819/79036730-587ccf80-7bf5-11ea-9193-b82c7c9680e2.png)

## Soal 3
Source code: [soal3.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal3/soal3.c)
### Penjelasan soal 
Buat sebuah program C untuk mengkategorikan file. Program ini akan
memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah
sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working
directory ketika program kategori tersebut dijalankan.

Ada 3 opsi untuk menjalankan program:
* opsi -f: digunakan untuk mengkategorikan file untuk file-file tertentu sebagai argumen.
* opsi * : digunakan untuk mengkategorikan file untuk seluruh file dalam current working directory saat program dijalankan.
* opsi -d: digunakan untuk mengkategorikan file dalam directory tertentu sebagai argumen.
### Penyelesaian
* Copy path current working directory
```c++
getcwd(cwd, sizeof(cwd));
```
* Cek argumen yang dimasukkan benar atau salah
```
if(argc < 2) {
	printf("Argumen invalid");
	return 0;
}
```
* Untuk opsi `-f`
* Loop untuk membuat thread sejumlah file
* Menunggu thread selesai dijalankan

```c++
if(strcmp(argv[1], "-f") == 0) {
	for(int i=2; i<argc; i++) {
		err = pthread_create(&tid[i], NULL, move, (void *)argv[i]);
		if(err != 0) 
			printf("\ncan't create thread : [%s]",strerror(err));
	}
	for(int j=2; j<argc; j++)
		pthread_join(tid[j], NULL);
}
```
* Selain opsi `-f`
* Jika opsi `-d` buka direktori yang ada di `argv[2]` dan copy ke array `folder`
```c++
if(strcmp(argv[1], "-d") == 0) {
	dir = opendir(argv[2]);
	strcpy(folder, argv[2]);
}
```
* Jika opsi * buka current working directory
```c++
else if((argv[1][0]=='*') && (strlen(argv[1])==1)) {
	dir = opendir(cwd);
```
* Agar tidak menyertakan direktori, file `soal3.c` dan file `soal3`
```c++
if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) 
continue;
```
* Buat thread
```c++
err = pthread_create(&tid[i], NULL, move, tmp->d_name);
```
* Tunggu thread selesai dan closedir
```c++
for(int j=0; j<i; j++)
	pthread_join(tid[j], NULL);
closedir(dir);
```

* Fungsi `move`
* Cek extension file
* Jika ada extension, jadikan huruf kecil dan masukkan ke array `ext`
* Jika tidak ada extension maka array `ext` diisi "Unknown"
```c++
int dot = '.';
char *extension = NULL;
extension = strrchr(filepath, dot);

char ext[1000];
memset(ext, '\0', sizeof(ext));
if(extension) {
	extension++;
	for(int i=0; i<strlen(extension); i++) {
		ext[i] = tolower(extension[i]);
	}
}
else strcpy(ext, "Unknown");
```
* Untuk mengambil nama file
```c++
int slash = '/';
char *filename = NULL;
filename = strrchr(filepath, slash);
if(filename) 
	filename++;
else filename = filepath;
```
* Untuk membuat path direktori extension yang akan dibuat
```c++
char folderpath[1000];
strcpy(folderpath, cwd);
strcat(folderpath, "/");
strcat(folderpath, extlower);
```
* Buat direktori
```c++
mkdir(folderpath, S_IRWXU);
```
* Untuk opsi `-d`
* Array `fullname` untuk menyimpan path file
* Pindahkan file dengan path yang ada pada `fullname` ke direktori yang ada pada `folderpath`
```c++
char fullname[1000];
strcpy(fullname, folder);
strcat(fullname, "/");
strcat(fullname, filename);

strcat(folderpath, "/");
strcat(folderpath, filename);

rename(fullname, folderpath);
```
* Untuk opsi *
```c++
strcat(folderpath, "/");
strcat(folderpath, filename);

rename(filepath, folderpath);
```
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
