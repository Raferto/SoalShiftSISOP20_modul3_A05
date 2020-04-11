# SoalShiftSISOP20_modul3_A05
**Intan Kusuma R  (05111840000020)**

**Rafid Ferdianto (05111840000032)**

## Soal 1

- belum selesai

**Kendala**

- tidak begitu memahami soal dan implementasinya

## Soal 2
Source code: [soal2_client.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal2/soal2_client.c)
	     [soal2_server.c](https://github.com/Raferto/SoalShiftSISOP20_modul3_A05/blob/master/soal2/soal2_server.c)
### Penjelasan soal dan penyelesaian
Soal ini meminta kita membuat game online berbasis text console dengan menggunakan 2 program, 1 client dan 1 server. Pada server hanya akan menampilkan tulisan “Auth success”, “Auth failed”, dan list akun. Sedangkan, pada Client Side terdapat 2 screen sebagai berikut :

**Screen 1**

Pada menu ini player bisa memilih menu login atau register, setelah memilih menu akan dihubungkan ke server sesuai pilihan. Apabila memilih menu login maka client side akan mengirim sinyal i ke server beserta username dan password untuk di cek apakah terdaftar atau tidak. Apabila terdaftar akan mengrimkan sinyal s agar client bisa lanjut ke screen 2. Sedangkan apabila memilih menu register maka akan mengirim sinyal r beserta username dan password ke server untuk didaftakan ke akun.txt (dan array akun).

**Client Side**
```c++
bool screen1(){
    char buffer[1024] = {0};
    char in[1024],akun[1024];
    
    printf("\n1. Login\n2. Register\nChoices : ");
    scanf("%s", in);
    if(strcmp(in, "login")==0){
        send(sock , "i", 1, 0 );

        printf("Username: ");
        scanf("%s",akun);
        printf("Password: ");
        scanf("%s",in);
        sprintf(akun,"%s %s",akun,in);

        send(sock , akun, strlen(akun), 0 );
        recv(sock , buffer , 1 , 0);
        if(strcmp(buffer, "s")==0){
            printf("login success \n");
            return true;
        }
        else
            printf("login failed\n");
    }
    else if(strcmp(in, "register")==0){
        send(sock , "r", 1, 0 );

        printf("Username: ");
        scanf("%s",akun);
        printf("Password: ");
        scanf("%s",in);
        sprintf(akun,"%s %s",akun,in);

        send(sock , akun, strlen(akun), 0 );
        printf("register success\n");
    }
    return false;
}
```

**Server Side**
```c++
 while(1){
        char buffer[1024]={0};
        recv(socket , buffer , 1 , 0);

        if(strcmp(buffer,"i")==0){
            bool login = false;
            recv(socket , buffer , 30 , 0);

            for(i=0;i<n;i++){
                if(strcmp(buffer,akun[i])==0){
                    login = true;
                    break;
                }
            }
            if(login){
                printf("Auth success\n");
                send(socket , "s" , 1 , 0 );
            }
            else{
                printf("Auth Failed\n");
                send(socket , "f" , 1 , 0 );
            }
        }

        else if(strcmp(buffer, "r")==0){
            recv(socket , buffer , 30 , 0);
            FILE *fAkun = fopen("akun.txt", "r+");

            printf("Username Password\n");
            char c = fgetc(fAkun);
            while (c != EOF){
                printf("%c",c );
                c = fgetc(fAkun);
            }

            fputs(buffer,fAkun);
            fputs("\n",fAkun);
            fclose(fAkun);

            strcpy(akun[n++],buffer);
            printf("%s\n\n",buffer );

        }
}
```


**Screen 2**

Pada menu ini player bisa memilih menu find atau logout. Apabila memilih menu logout tinggal dikembalikan ke screen 1 (tidak perlu terhubung ke server), sedangkan untuk menu find, dihunbungkan ke server untuk mencari player lain. Apabila belum ada player lain maka akan dibuatkan room baru (struct) dan player 1 diisi thread dari player ini. Apabila sudah ada player lain (yangg menunggu) maka langsung dihubungkan ke room yang sudah ada dan player 2 diisi thread dari player ini. Kemudian kedua player akan diberi signal oleh server untuk memulai pertandingan. Pada setiap player digunakan 2 thread, satu untuk menyerang dan satu lagi apabila terkena damage. thread menyerang mengirim signal ke server dan selanjutnya akan dikirim ke lawan bahwa healthnya telah berkurang. Apabila player sudah ada yang kalah(health = 0) maka player tersebut akan mengirim signal ke server agar memberi tahu bahwa game telah selesai dan player yang satunya menang. Setelah selesai kedua player akan kembali ke screen 2 lagi.

**Client Side**
```c++
bool screen2(){
    char buffer[1024] = {0},
         in[1024] , c;

    printf("\n1. Find Match\n2. Logout\nChoices : ");
    scanf("%s",in);
    if(strcmp(in, "logout")==0)
        return false;
    
    else if(strcmp(in, "find")==0){
        pthread_t a,d,w;

        send(sock , "p" , 1 , 0 );
        ingame = true;

        recv(sock , buffer , 1 , 0);
        if(strcmp(buffer, "w")==0){
            ingame = false;
            if(pthread_create(&w,NULL,wait,NULL) != 0){
                perror("threads");
                exit(1);
            }
        }
        while(!ingame)
            printf("Waiting for player ...\n");
        
        pthread_join(w,NULL);
            
        if(pthread_create(&a,NULL,attack,NULL) != 0){
            perror("threads");
            exit(1);
        }
        if(pthread_create(&d,NULL,defend,NULL) != 0){
            perror("threads");
            exit(1);
        }

        pthread_join(a,NULL);
        pthread_join(d,NULL);
    }

    return true;
}
```
**Thread attack pada Client Side**
```c++
void *attack(){
    char c;
    printf("Game dimulai silahkan tap tap secepat mungkin !!\n");

    while(ingame){
        system("stty raw");
        c=getchar();
        if(c == ' ' && ingame){
            printf("hit!!");
            send(sock , "a", 1, 0);
        }
    }
    system("stty cooked");
    return NULL;
}
```
**Thread diserang pada Client Side**
```c++
void *defend(){
    health = 100;
    while(ingame){
        char buffer[1024]={0};
        recv(sock , buffer , 1 , 0);
        system("stty cooked");
        if(strcmp(buffer, "a")==0){
            health-=10;
            if(health)
                printf("\nHealth : %d\n",health);
            else{
                ingame = false;
                send(sock , "x", 1, 0);
                printf("\nGame berakhir kamu kalah\n");
                recv(sock , buffer , 1 , 0);
            }
        }
        else{
            ingame = false;
            send(sock , "x", 1, 0);
            printf("\nGame berakhir kamu menang\n");
        }
        system("stty raw");
    }
    return NULL;
}
```
**Server Side**
```c++
while(1){
        char buffer[1024]={0};
        recv(socket , buffer , 1 , 0);

        else{
		int *ingame = (int*) malloc(sizeof(int));
		*ingame = 1;
		match = (game*) malloc(sizeof(game));
		match->p1 = pthread_arg;
		match->status = ingame;

		status = 1;

		send(socket , "w" , 1 , 0 );
		while(*ingame);
		send(socket , "r" , 1 , 0 );

		int e_socket = match->p2->new_socket_fd;
		while(!(*ingame)){
		    recv(socket , buffer , 1 , 0);
		    if(strcmp(buffer, "a")==0)
			send(e_socket , "a" , 1 , 0 );
		    else{
			*ingame = 1;
			send(e_socket , "x" , 1 , 0 );
		    }
		}
        }
}

```


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

* Kurang waktu untuk mengerjakan

**Screenshot Hasil**
* Opsi `-d`

![-d1](https://user-images.githubusercontent.com/61036923/79040982-7e669c00-7c16-11ea-8c1e-fabedbc2f29a.png)
![-d2](https://user-images.githubusercontent.com/61036923/79040988-845c7d00-7c16-11ea-8101-f0a8fd7e2a56.png)
![-d3](https://user-images.githubusercontent.com/61036923/79040990-86bed700-7c16-11ea-88c1-2c5bbfa51477.png)
![-d4](https://user-images.githubusercontent.com/61036923/79040992-8aeaf480-7c16-11ea-8adf-7efa2ed6d9a0.png)

* Opsi `-f`

![-f1](https://user-images.githubusercontent.com/61036923/79040995-8fafa880-7c16-11ea-859e-3278f64cbd0f.png)
![-f2](https://user-images.githubusercontent.com/61036923/79040963-6262fa80-7c16-11ea-97f3-518110f26c02.png)
![-f3](https://user-images.githubusercontent.com/61036923/79040969-6bec6280-7c16-11ea-9110-a07ea64cfd6b.png)
![-f4](https://user-images.githubusercontent.com/61036923/79040971-6ee75300-7c16-11ea-85b4-ca4f8e2ad854.png)

* Opsi `*`

![star1](https://user-images.githubusercontent.com/61036923/79040973-70b11680-7c16-11ea-918e-22da3ea80c14.png)
![star2](https://user-images.githubusercontent.com/61036923/79040980-79a1e800-7c16-11ea-9113-20ef2b3424f2.png)


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

* Ada hasil perkalian yang salah

**Screenshot Hasil**

* Soal4a

![4a](https://user-images.githubusercontent.com/61036923/79041202-1749e700-7c18-11ea-8e27-6796acff933c.png)

* Soal4b

![4b](https://user-images.githubusercontent.com/61036923/79041209-27fa5d00-7c18-11ea-973d-b81960483f52.png)

* Soal4c

![4c](https://user-images.githubusercontent.com/61036923/79041214-2f216b00-7c18-11ea-9c41-5c1e7405e049.png)
