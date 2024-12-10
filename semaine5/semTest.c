#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h> 


/* syncronise des processus grace a des semaphores*/
int main (void)
{
    pid_t pid;
    int P2, clef, sema;
    char i;
    unsigned short tab_sema[2];
    struct sembuf operation[1];
    FILE *p;

    p = fopen("fic3.txt", "w");
    if(p == NULL)
    {
        perror("pb ouverture fichier");
        exit(0);
    }
    clef = ftok("fic3.txt", 4);
    /*                deuxieme element de table volorisé                 */ 
    tab_sema[0] = 0;
    tab_sema[1] = 1;
    /*                déclaration d'une variable sémaphore                */ 
    sema = semget(clef, 2, IPC_CREAT|0600);
    if(sema == -1)
    {
        perror("pb semget");
        exit(0);
    }

    /*                initialisation du sémaphore avec les valeurs initiales  */
    semctl(sema,2,SETALL,tab_sema); 
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if(pid == 0)
    {
        for (i='a';i<'z'+1;i++)
        {
            operation[0].sem_num=0;
            operation[0].sem_flg=0;
            operation[0].sem_op=-1;
            // demande l'acces sur le 0
            // attend jusqu'a ce que libre
            semop(sema,operation,1);
            fprintf(p,"%c",i);
            fflush(p);

            operation[0].sem_num=1;
            operation[0].sem_flg=0;
            operation[0].sem_op=+1;
            // donne un point d'acces au 1
            semop(sema,operation,1);
        }
    }
    if(pid > 1)
    {
        for (i='A';i<'Z'+1;i++)
        {
            operation[0].sem_num=1;
            operation[0].sem_flg=0;
            operation[0].sem_op=-1;
            // demande l'acces sur le 1
            // attend jusqu'a ce que libre
            semop(sema,operation,1);
            fprintf(p,"%c",i);
            fflush(p);

            operation[0].sem_num=0;
            operation[0].sem_flg=0;
            operation[0].sem_op=+1;
            // donne un point d'acces au 0
            semop(sema,operation,1);
        }   
    }
    fclose(p);
    return 0;
}
