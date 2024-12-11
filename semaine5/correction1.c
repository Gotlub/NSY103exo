#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
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
    do
    {
        msgrcv(id, &rcvbuf, sizeof(struct msgbuf), 0, 0);
        if(rcvbuf.texte[0] != 10 && rcvbuf.texte[0] != 1)
        {
            //sans compter le char de fin de saisie 10
            somme += strlen(rcvbuf.texte) - 1;
            nb++;
        }
    } while(rcvbuf.texte[0] != 1);
    printf("Nombre total d'octets lus : %d, nombre de message : %d \n", somme, nb);
    exit(0);
}

void main(void)
{
    struct msgbuf mybuf;

    id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if(fork() == 0)
        fils();
    mybuf.type = 0;
    do
    { 
        printf("Enter your message, empty for exit : \n");
        fgets(mybuf.texte, 256, stdin);
        msgsnd(id, &mybuf, sizeof(struct msgbuf), 0);
    //  10 = Line Feed (saut de ligne), la fin de saisie de stdin 
    }while(mybuf.texte[0] != 10);
    // si le premier message a pour unique char 0, chez moi le programme plante a la reception
    // donc 1 est utilisé a la place
    mybuf.texte[0] = 1;
    msgsnd(id, &mybuf, sizeof(struct msgbuf), 0);
    wait(NULL);
}
