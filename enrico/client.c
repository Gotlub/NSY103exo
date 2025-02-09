#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>

int main(void)
{
    // Déclaration de la structure sockaddr_in pour l'adresse client et serveur
    struct sockaddr_in ClientAdr, serveurAdr;

    // Initialisation de l'adresse du serveur
    bzero(&serveurAdr, sizeof(serveurAdr));
    serveurAdr.sin_family = AF_INET;                // Famille d'adresses Internet
    serveurAdr.sin_port = htons(10000);             // Port du serveur
    serveurAdr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Adresse IP du serveur (localhost)

    int id, connectSock, i, W, R, nbre=0;
    char tableau[256] = "BONJOUR TOUT LE MONDE";        // Message à envoyer
    socklen_t taille = sizeof(ClientAdr);        // Taille de la structure sockaddr_in

    // Création du socket
    id = socket(PF_INET, SOCK_STREAM, 0); 
    if(id < 0)
    {
        perror("Erreur de création du socket");
        exit(1);  // Sortie si erreur
    }

    // Connexion au serveur
    connectSock = connect(id, (struct sockaddr*)&serveurAdr, taille);
    if(connectSock < 0)
    {
        perror("Erreur de connexion");
        exit(1);  // Sortie si erreur
    }

    // Envoi et réception des données
    for(i = 0; i < (int)strlen(tableau); i++)
    {
        // Envoi d'un caractère à la fois
        W = write(id, &tableau[i], 1);
        printf("Affichage des caractéres envoyés par le client :%c\n",tableau[i]);
        if(W < 0)
        {
            perror("Erreur d'envoi");
            exit(1);  // Sortie si erreur
        }

        // Réception d'un caractère du serveur
        R = read(id, &tableau[i], 1);
        printf("Affichage des caractéres reçut par le client :%c\n",tableau[i]);
        nbre=nbre+R;
        if(R < 0)
        {
            perror("Erreur de lecture");
            exit(1);  // Sortie si erreur
        }

        // Affichage du nombre d'octets envoyés et reçus
        printf("Nbre d'octets envoyés : %d ; Nbre d'octets reçus : %d\n", W, R);
    }
    printf("TOTAL d'octets reçus : %d\n", nbre);

    // Fermeture du socket
    close(id);
    return 0;
}
