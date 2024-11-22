#include <stdio.h>
#include <time.h>
#include <pthread.h>
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

void *lautcher(void *arg)
{
    arg_t *params = (arg_t *) arg;
    int k = params->k;
    int l = params->l;
    printf("%d %d \n", k, l);
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
    pthread_t tid1;
	pthread_t tid2;
    arg_t arg;
    arg.k = 0;
    arg.l =  V / 3;
    time_t start = time (NULL);
    pthread_create(&tid1, NULL, lautcher, (void*)&arg);
    if(pthread_join(tid1, NULL) != 0){
        return 1;
    }
    arg.k = V / 3 ;
    arg.l = (V / 3) * 2;
    pthread_create(&tid2, NULL, lautcher, (void*)&arg);
    pthread_join(tid2, NULL);
    arg.k = (V / 3) * 2 ;
    arg.l = V;
    *lautcher((void*)&arg);
    printf ("Duree = %ds\n", (int) (time (NULL) - start));
    printSolution();
    return 0;
}
