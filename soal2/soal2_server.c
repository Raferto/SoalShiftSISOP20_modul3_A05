#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#define PORT 8080
#define MAX_PLAYER 1000
#define MAX_MATCH 1000

typedef struct pthread_arg_t {
    int new_socket_fd;
    int player_id;
    struct sockaddr_in client_address;
} pthread_arg_t;

typedef struct game{
    pthread_arg_t *p1, *p2;
    int *status;
} game;

game *match;

char akun[MAX_PLAYER][30];
int n;

int status = 0;


int load(){
    FILE  *fAkun;
    char in[1024];
    int i=0;

    if( fAkun = fopen("akun.txt", "r") )
        while(fscanf(fAkun, "%s ", akun[i]) == 1){
            fscanf(fAkun, "%s\n", in);
            sprintf(akun[i],"%s %s",akun[i],in);
            i++;
        }
    else
        fAkun = fopen("akun.txt", "w");
    fclose(fAkun);
    return i;
}

void *pthread_routine(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int socket = pthread_arg->new_socket_fd;
    int id = pthread_arg->player_id;
    struct sockaddr_in client_address = pthread_arg->client_address;

    int i;
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

        else if(strcmp(buffer, "p")==0){
            if(status){
                match->p2 = pthread_arg;
                int *ingame = match->status;
                int e_socket = match->p1->new_socket_fd;

                *ingame = 0;
                send(socket , "r" , 1 , 0 );

                status = 0;
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
    }

    return NULL;
}

int main(int argc, char const *argv[]) {
    int port, socket_fd, new_socket_fd;
    struct sockaddr_in address;
    pthread_attr_t pthread_attr;
    pthread_arg_t *pthread_arg;
    pthread_t pthread;
    socklen_t client_address_len;

    memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    /* Create TCP socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Bind address to socket. */
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof address) == -1) {
        perror("bind");
        exit(1);
    }

    /* Listen on socket. */
    if (listen(socket_fd, 3) == -1) {
        perror("listen");
        exit(1);
    }

    /* Initialise pthread attribute to create detached threads. */
    if (pthread_attr_init(&pthread_attr) != 0) {
        perror("pthread_attr_init");
        exit(1);
    }
    if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread_attr_setdetachstate");
        exit(1);
    }

    int i;
    n = load();

    for(i=0; ; i++){
        pthread_arg = (pthread_arg_t *)malloc(sizeof *pthread_arg);
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }

        client_address_len = sizeof pthread_arg->client_address;
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("accept");
            free(pthread_arg);
            continue;
        }

        pthread_arg->new_socket_fd = new_socket_fd;
        pthread_arg->player_id = i;

        if (pthread_create(&pthread, &pthread_attr, pthread_routine, (void *)pthread_arg) != 0) {
            perror("pthread_create");
            free(pthread_arg);
            continue;
        }
    }
}