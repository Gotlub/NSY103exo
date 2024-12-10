#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void)
{
    pid_t pid;
    int status;
    char c;
    int number = 5;
    char chaine[6] = "";
    FILE* fichier = NULL;
    if (mknod("tube.txt", S_IFIFO|0755, 0) == -1)
        perror("mknod");
    pid = fork();
    if (pid == -1)
        return (1);
    printf("pid : %d \n", pid);
    if (pid == 0)
    {
        fichier = fopen("tube.txt", "w");
        if(fichier==NULL){
            printf("Erreur lors de l'ouverture d'un fichier");
            exit(1);
        }
        for(int i = 0; i < number; i++)
        {
            chaine[i] = 65 + i % 26;
        }
        printf("%s \n", chaine);
        fprintf(fichier, "%s", chaine) ;
        fclose(fichier);
        exit(0);
    }
    if (pid > 0)
    {
        
        wait(&status);
        fichier = fopen("tube.txt", "r");
        if(fichier==NULL){
            printf("Erreur lors de l'ouverture d'un fichier");
            exit(1);
        }
        fgets(chaine,number + 1 , fichier);
        printf("%s\n", chaine);
        fgets(chaine, 1 , fichier);
        printf("%s", chaine);
        fclose(fichier);
        printf("retourn 0");
    }
    return (0);
}
