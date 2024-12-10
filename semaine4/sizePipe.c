#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <fcntl.h>

//gcc -D_GNU_SOURCE nomduprogramme.c
int main(void)
{
    pid_t pid;
    int tube[2];
    int status;
    if (pipe(tube) == -1)
        perror("pipe");
    printf("Taille max du buffer  %d \n",  fcntl(tube[0], F_GETPIPE_SZ));

    return 0;
}
