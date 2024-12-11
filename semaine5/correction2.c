#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int id;

void fils(int idfils)
{
	int *zone;
	zone=(int*)(shmat(id,NULL,0)+(idfils-1)*sizeof(int));
	*zone=5000*idfils;
}

int main(void)
{
	int *zone;
	id=shmget(IPC_PRIVATE,4*sizeof(int),IPC_CREAT|0777);
	if(fork()==0)
	{
		fils(1);
		exit(0);
	}
	if(fork()==0)
	{
		fils(2);
		exit(0);
	}
	if(fork()==0)
	{
		fils(3);
		exit(0);
	}
	if(fork()==0)
	{
		fils(4);
		exit(0);
	}
	zone=(int*)shmat(id,NULL,SHM_RDONLY);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	printf("Voici les quatre entiers : %d, %d, %d et %d\n",zone[0],
		zone[1],zone[2], zone[3]);
	shmdt(zone);
}
