#include<stdio.h>
#include<pthread.h>

float res[3];
pthread_t id1,id2;

void* Calcul1(void *i)
{
    res[0]=(float)1+5;
    printf("Thread 1 : calcul intermédiaire terminé.\n");
    //vide le tampon de sortie, pourquoi ?
    fflush(stdout);
    pthread_exit(NULL);
}
void* Calcul2(void *i)
{
    res[1]=(float)6-2;
    printf("Thread 2 : calcul intermédiaire terminé.\n");
    fflush(stdout);
    pthread_exit(NULL);
}
int main(void)
{
    float calcul;
    if(pthread_create(&id1,NULL,Calcul1,NULL))
        printf("Erreur création thread 1...\n");
    if(pthread_create(&id2,NULL,Calcul2,NULL))
        printf("Erreur création thread 2...\n");
    res[2]=8+6;
    printf("Thread principal : calcul intermédiaire terminé.\n");
    //vide le tampon de sortie, pourquoi ?
    fflush(stdout);
    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    calcul=(res[0]*(res[1]+1))/res[2];
    printf ("Thread principal : résultat du calcul = %f\n",calcul);
}
