#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/sem.h> 
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    key_t key, key2;
    int shmid, sema, j;
    struct sembuf operation[1];
    pid_t pid[4] = { 0, 0, 0, 0};
    unsigned short tab_sema[4] = { 1, 0, 0, 0};
    key = ftok("TP2.c", 65);
    key2 = ftok("TP2.c", 3);
    /*                déclaration d'une variable sémaphore                */
    sema = semget(key2, 4, IPC_CREAT|0600);
    // shmget returns an identifier in shmid
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    // shmat to attach to shared memory
    char* str = (char*)shmat(shmid, NULL, 0);
    /*                initialisation du sémaphore avec les valeurs initiales  */
    semctl(sema, 4, SETALL, tab_sema);
    /*                  creation de 4 processus                                     */
    j = 4;
    for(int k = 0; k < 4; k++)
    {
        if(k == 0 || pid[k - 1] > 0)
        {
            pid[k] = fork();
            if (pid[k] == 0 && k < j)
            {
                j = k;
            }
        }
    }
    //si j == 4 on est dans le père
    if (j == 4)
    {
        for(int k = 0; k < 4; k++)
        {
            wait(NULL);
        }
        printf("Data read from memory:\n%s ", str);
        
        shmdt(str);
    }else{
        int lenStr;
        
        //demande d'acces a la resource
        operation[0].sem_num = j;
        operation[0].sem_flg = 0;
        operation[0].sem_op = -1;

        semop(sema,operation,1);
        lenStr = strlen(str);
        printf("Fork n° %d Write Data : \n", j);
        fgets(str + lenStr , 1024 - lenStr, stdin);
        shmdt(str);
        //Augmentation de sem_op du fork suivant
        operation[0].sem_num = (j + 1) % 4;
        operation[0].sem_flg = 0;
        operation[0].sem_op = +1;

        semop(sema,operation,1);
        exit(0);
    }
    // free the memorie
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
