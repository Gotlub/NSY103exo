#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int id;

struct msgbuf
{
    int type;
    char texte[256];
};


void fils(void)
{
	int nb = 0;
    int somme = 0;
	struct msgbuf rcvbuf;
	bool nonEmptyMessage=true;
	do
	{
		msgrcv(id,&rcvbuf,sizeof(struct msgbuf),0,0);
		nonEmptyMessage=rcvbuf.texte[0]!=1;
        nb = strlen(rcvbuf.texte);
		if (nonEmptyMessage)
			somme+=nb;
	} while(nonEmptyMessage);
	printf("Nombre total d'octets lus : %d\n",somme);
	exit(0);
}

void main(void)
{
    struct msgbuf mybuf;

    id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if(fork() == 0)
        fils();
    mybuf.type = 0;
    printf("Entre puis ctrl + d pour terminer la saisie :\n");
    while(fgets(mybuf.texte, 256, stdin))
    { 
        msgsnd(id, &mybuf, sizeof(struct msgbuf), 0);
    }
    // si le premier message a un unique char, avec 0 en deuxième message
    // chez moi le programme plante a la reception 
    // ( mybuf.texte constitué que de 0 ?)
    //  1 est utilisé a la place
    mybuf.texte[0] = 1;
    msgsnd(id, &mybuf, sizeof(struct msgbuf), 0);
    wait(NULL);
}
