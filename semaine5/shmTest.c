#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main()
{
    key_t key;
    int shmid;
    pid_t pid;
    key = ftok("shmTest.c", 65);
    pid = fork();
    if (pid == 0)
    {
        // shmget returns an identifier in shmid
        shmid = shmget(key, 1024, 0);

        // shmat to attach to shared memory
        char* str = (char*)shmat(shmid, NULL, 0);
        while (!strlen(str))
        {
            sleep(0.01);
        }
        printf("Data read from memory: %s \n", str);


        // detach from shared memory
        shmdt(str);

        return 0;
    } else {
        // shmget returns an identifier in shmid
        shmid = shmget(key, 1024, 0666 | IPC_CREAT);

        // shmat to attach to shared memory
        char* str = (char*)shmat(shmid, NULL, 0);

        printf("Write Data : \n");
        fgets(str, 1024, stdin);
        printf(" Data :  %s \n", str);
        shmdt(str);
    }
    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
