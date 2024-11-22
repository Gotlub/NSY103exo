#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// Number of vertices in the graph
#define V 6

#define INF 99999


int graph[V][V] = { { 0, 3, INF, INF, INF, INF},
                        { INF, 0, 8, 12, 4, INF },
                        { 2, INF, 0, 5, INF, INF },
                        { INF, INF, INF, 0, INF, INF},
                        { INF, INF, INF, INF, 0, 3 },
                        { INF, INF, INF, 8, INF, 0} };

typedef struct args {
    int k;
    int l;
}arg_t;

// Solves the all-pairs shortest path
// problem using Floyd Warshall algorithm
void partOfFloydWarshall(int k)
{
    int i, j;
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (graph[i][k] + graph[k][j] < graph[i][j])
                graph[i][j] = graph[i][k] + graph[k][j];
        }
    }
}

void lautcher(void *arg)
{
    arg_t *params = (arg_t *) arg;
    int k = params->k;
    int l = params->l;
    for(int i = k; i < l; i++) {
        partOfFloydWarshall( i);
    }
}

/* A utility function to print solution */
void printSolution()
{
    printf(
        "The following matrix shows the shortest distances"
        " between every pair of vertices \n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", graph[i][j]);
        }
        printf("\n");
    }
}

// driver's code
int main()
{
    pid_t pid;
    pid_t pid2;
    arg_t arg;
    int buf;
    int tube[2];
    int tube2[2];
    int	status;
    if (pid == -1) {
		// Il y a une erreur
		perror("fork");
		return 1;
	}
    
    if (pipe(tube)==-1){
		printf("pipe failed\n");
		return 1;
	}
    if (pipe(tube2)==-1){
		printf("pipe failed\n");
		return 1;
	}
    
    time_t start = time (NULL);
    pid = fork();
    if(pid == 0)
    {
        int i, j, h;
        arg.k = 0;
        arg.l =  V / 3;
        // Fermeture de lecture
        close(tube[0]);
        lautcher((void*)&arg);
        printf("Processus fils pid: %d, son pid : %d \n", getpid(), pid);
        for (i = 0; i < V; i++)
        {
            for (j = 0; j < V; j++)
            {
                buf = graph[i][j];
                write(tube[1], &buf, sizeof(int));
            }
        }
        // Fermeture de l'écriture
        close(tube[1]);
        printSolution();
        return 0;
    }
    pid2 = fork();
    if(pid2 == 0 )
    {
        // Fermeture de lecture
        close(tube2[0]);
        //Attente de la fin du processus fils
        waitpid(pid, &status, 0);
        printf("Processus père pid: %d, son pid : %d \n", getpid(), pid);
        for (int i = 0; i < V * V; i++)
        {
            read(tube[0], &buf, sizeof(int));
            graph[i%V][i/V] = buf;
        }
        arg.k = V / 3 ;
        arg.l = (V / 3) * 2;
        // Function call
        lautcher((void*)&arg);
         for (int i = 0; i < V * V; i++)
        {
            buf = graph[i/V][i%V];
            write(tube2[1], &buf, sizeof(int));
        }
        close(tube2[1]);
        printSolution();
        return 0;
    }
    if(pid2 > 0 && pid > 0)
    {
        
        //Attente de la fin du processus fils
        waitpid(pid2, &status, 0);
        // Fermeture de l'ecriture
        close(tube[1]);
        close(tube2[1]);
        for (int i = 0; i < V * V; i++)
        {
            read(tube2[0], &buf, sizeof(int));
            graph[i%V][i/V] = buf;
        }
        arg.k = (V / 3) * 2 ;
        arg.l = V;
        lautcher((void*)&arg);
        printf ("Duree = %ds\n", (int) (time (NULL) - start));
        printSolution();
    }
    return 0;
}
