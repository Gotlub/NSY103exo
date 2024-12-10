#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<signal.h>
#include<sys/types.h>

void ArretDuTravail(int sig)
{
	printf("Fin du fils.\n");
	exit(0);
}

int main(void)
{
	int p[2],n,f;
	pipe(p);
	f=fork();
	if(f==0)
	{
		signal(SIGPIPE,ArretDuTravail);
		printf("Début du fils...\n");
		close(p[0]);
		srand(time(NULL));
		while(1)
		{
			n=(int)(100*((float)rand()/(float)RAND_MAX));
			write(p[1],&n,sizeof(int));
		}
	}
	else
	{
		int i;
		close(p[1]);
		for(i=0;i<10;i++)
		{
			read(p[0],&n,sizeof(int));
			printf("Père : j'ai lu %d\n",n);
		}
		close(p[0]);
	}
	return 0;
}
