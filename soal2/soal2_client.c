#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#define PORT 8080

int sock = 0;

bool ingame = false;
int health = 100;

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

void *wait(){
    char buffer[1024]={0};
    recv(sock , buffer , 1 , 0);
    ingame = true;

    return NULL;
}

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

int main(int argc, char const *argv[]) {

    struct sockaddr_in address;
    struct sockaddr_in serv_addr;

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    bool screen = false;
    while(1){
        if(screen)
            screen = screen2();
        else
            screen = screen1();
    }
}