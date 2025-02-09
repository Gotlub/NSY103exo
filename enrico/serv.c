#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdbool.h>


#define PORT 10000

int main(void) {
    int idsoc, nbr, i, sc;
    char buffer[256];
    struct sockaddr_in adrServeur ;
    socklen_t tailleCl = sizeof(struct sockaddr_in);
    bool fin;

    /*******************************************/
    /* Configuration de l'adresse du serveur   */
    /*******************************************/
    adrServeur.sin_family = AF_INET;            // Utilisation du protocole IPv4
    adrServeur.sin_port = htons(PORT);         // Port du serveur (10000)
    adrServeur.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY;    // Accepte toutes les interfaces réseau (INADDR_ANY)

    /*******************************************/
    /* Création de la socket serveur           */
    /*******************************************/
    idsoc = socket(PF_INET, SOCK_STREAM, 0);    // Création d'un socket de type STREAM (TCP)
    if (idsoc == -1) {
        perror("Erreur création socket");       // Gestion de l'erreur si la création échoue
        exit(EXIT_FAILURE);                      // Arrêt du programme en cas d'erreur
    }

    /*******************************************/
    /* Liaison de la socket à l'adresse        */
    /*******************************************/
    int opt = 1;
    setsockopt(idsoc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Option pour permettre la réutilisation de l'adresse

    if (bind(idsoc, (struct sockaddr*)&adrServeur, sizeof(adrServeur)) == -1) {
        perror("Erreur bind");                   // Gestion de l'erreur si bind échoue
        close(idsoc);                             // Fermeture du socket avant d'arrêter le programme
        exit(EXIT_FAILURE);
    }

    /*******************************************/
    /* Mise en écoute de la socket             */
    /*******************************************/
    if (listen(idsoc, 5) == -1) {                // Mise en écoute du serveur avec une file d'attente de 5 connexions
        perror("Erreur listen");                 // Gestion de l'erreur si listen échoue
        close(idsoc);
        exit(EXIT_FAILURE);
    }
    printf("Serveur en attente sur 127.0.0.1: %d...\n",(int)PORT);
    /*******************************************/
    /* Boucle principale d'attente de clients  */
    /*******************************************/
    while (1) {
        struct sockaddr_in adrClient;           // Structure pour stocker l'adresse du client
        
        // Acceptation d'une connexion entrante
        sc = accept(idsoc, (struct sockaddr*)&adrClient, &tailleCl);
        if (sc == -1) {
            perror("Erreur accept");             // Gestion de l'erreur si accept échoue
            continue;                             // Reprise de la boucle pour accepter un autre client
        }
        if (fork() == 0) {
            return 0;
        }
        printf("Nouveau client connecté: %s:%d\n", 
               inet_ntoa(adrClient.sin_addr),      // Affichage de l'adresse IP du client
               ntohs(adrClient.sin_port));         // Affichage du port du client
        
        fin = false;

        /***************************************/
        /* Boucle de traitement du client      */
        /***************************************/
        while (!fin) {
            nbr = read(sc, buffer, sizeof(buffer)); // Lecture des données envoyées par le client
            if (nbr <= 0) {
                if (nbr == 0) printf("Client déconnecté\n"); // Si la lecture retourne 0, le client est déconnecté
                if (nbr < 0) perror("Erreur read");        // Si la lecture retourne une erreur
                break;                                      // Sortie de la boucle pour ce client
            }

            // Traitement de la donnée
            buffer[nbr] = '\0'; // Sécurité pour l'affichage (ajout de caractère de fin de chaîne)
            if (buffer[nbr - 1] == 0) fin = true; // Si le dernier caractère reçu est un '\0', terminer la boucle

            // Conversion majuscules -> minuscules
            for (i = 0; i < nbr; i++) {
                if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                    buffer[i] += 32; // Conversion d'une majuscule en minuscule
                printf("Affichage des caractères modifiés par le serveur : %c\n",buffer[i]);
                }
            }

            // Renvoi des données traitées au client
            if (write(sc, buffer, nbr) == -1) {
                perror("Erreur write"); // Gestion de l'erreur si l'envoi échoue
                break;
            }
        }

        close(sc); // Fermeture de la connexion avec ce client
    }

    close(idsoc); // Fermeture du socket serveur
    return 0;
}
