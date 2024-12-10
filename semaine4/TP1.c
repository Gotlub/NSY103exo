#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
//pour les macros
#include <sys/ioctl.h>

int main (void)
{
    int buf ;
    int tube [2] ;
    int err;
    int bytesAvailable;
    pid_t pid;
    int status;

    pipe (tube) ;
    if (pipe(tube)==-1)
    {
        printf("pipe failed\n");
        return 1;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if(pid == 0)
    {
        int i = 0;
        int random;
        while(1)
        {
            sleep(0.01);
            random = rand() % 101;
            write(tube[1], &random, sizeof(int));
            i++;
        }
    }
    if(pid > 1)
    {
        int tailleTube = rand() % 100 * 4;
        do {
            err = ioctl(tube[0], FIONREAD, &bytesAvailable);
        }while (bytesAvailable < tailleTube);
        printf("taille du pipe :  %d\n" ,bytesAvailable);
        kill(pid, SIGSTOP );
        int val = 0;
        for(int k = 0; k < bytesAvailable / sizeof(int) + 4; k++)
        {
            read(tube[0], &buf, sizeof(int));
            printf(" %d", buf);
        }
        printf("taille du pipe :  %d\n" ,bytesAvailable);
        sleep(1);
        kill(pid, SIGCONT);
        do {
            err = ioctl(tube[0], FIONREAD, &bytesAvailable);
        }while (bytesAvailable < tailleTube);
        kill(pid, SIGSTOP );
        printf("taille du pipe :  %d, taille du tube %d \n" ,bytesAvailable, tailleTube);
        kill(pid, 9);
    }
    return 0;
}
