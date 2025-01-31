#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(void)
{
    int f1,f2,w1,w2,r1,r2,r3;
    float calcul;
    f1=fork();
    // si f1 == 0 alors on est dans le fils du fork f1
    if(f1==0)
    {
        int res;
        res=1+5; // Calcul intermédiaire du fils 1
        printf("Fils1 : calcul terminé. pid : %d \n", getpid());
        exit(res);
    }
    f2=fork();
    // si f2 == 0 alors on est dans le fils du fork f2
    if(f2==0)
    {
        int res;
        res=6-2; // Calcul intermédiaire du fils 2
        printf("Fils2 : calcul terminé. pid : %d \n", getpid());
        exit(res);
    }
    // Si status n'est pas un pointeur nul, le status du processus fils (valeur retournée par exit())
    // est mémorisé à l'emplacement pointé par status, qui contient si le processus c'est terminé noramllement ou non, ça cause et sa valeur de sortie.
    // retourne le pid du processus fils si le retour est dû à la terminaison d'un processus fils
    w1=wait(&r1);
    w2=wait(&r2);
    //(si WIFEXITED (status) renvoie vrai) renvoie le code de retour du processus fils passé à
    // _exit() ou exit() ou la valeur retournée par la fonction main() 
    r1=WEXITSTATUS(r1);
    r2=WEXITSTATUS(r2);
    //w1 est l'indentifiant d'un processus arreté (retour de wait -> pid child f1), elle ne peu avoir du processus père (f1)
    //on ne rentre pas dans cette condition
    if(w1!=f1)
    {
        printf("Pere : en calcul. pid : %d \n", getpid());
        r3=r1;
        r1=r2;
        r2=r3;
    }
    r3=8+6; // Calcul intermédiaire du père
    calcul=(float)(r1*(r2+1))/(float)r3;
    printf("Résultat du calcul : %f\n",calcul);
}
