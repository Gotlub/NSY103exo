#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
//pour les macros
#include <sys/ioctl.h>

int main (void)
{
    srand( time( NULL ) );
    int tube[2];
    int tube2[2];
    pid_t pid;
    int i = 0;
    int err;
    int bytesAvailable;

    pipe(tube);
    pipe(tube2);
    if (pipe(tube) == -1 || pipe(tube2) == -1 ) 
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
        int random;

        while(1)
        {
            while ( !bytesAvailable)
            {
                err = ioctl(tube2[0], FIONREAD, &bytesAvailable);
            }
            read(tube2[0], &random, sizeof(int));
            random = rand() % 101;
            printf("entrée - n° %d : -%d  \n", i, random);
            write(tube[1], &random, sizeof(int));
            i++;
        }
    }
    if(pid > 1)
    {
        int buf;
        int tailleTube = (random() % 100);
        
        while (i < tailleTube)
        {
            write(tube2[1], &i, sizeof(int));
            while ( !bytesAvailable)
            {
                err = ioctl(tube[0], FIONREAD, &bytesAvailable);
            }
            printf("sorti n° %d", i);
            read(tube[0], &buf, sizeof(int));
            printf(" *%d  \n", buf);
            i++;
        }
        //
        
        //int r = read(tube[0], &buf2, 1);
        kill(pid, SIGKILL);
        err = ioctl(tube[0], FIONREAD, &bytesAvailable);
        printf("taille du pipe restant :  %d \n" , bytesAvailable);
        printf("tailleTube :  %d, i : %d \n" ,tailleTube , i);
    }
    return 0;
}
