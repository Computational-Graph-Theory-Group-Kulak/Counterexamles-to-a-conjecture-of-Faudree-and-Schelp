#include "readGraph/readGraph6.h"
#include "bitset.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

struct graph {
    int numberOfVertices;
    bitset *adjacencyList;
};

struct counters {
    long long unsigned int skippedGraphs;
};

void find_path_recurs(size_t n, size_t start_vertex, bitset current_path, size_t current_vertex, bitset* adj, int path_length, bitset** edges_found) {
    if(current_vertex  < start_vertex){
                edges_found[start_vertex][current_vertex] |= singleton(size(current_path)-1);
            }else{
                edges_found[current_vertex][start_vertex] |= singleton(size(current_path)-1);
            }
    if (path_length == 0){
        return;
    }
    bitset vertices_to_check = difference(adj[current_vertex],current_path);
    forEach(vertex, vertices_to_check) {
        find_path_recurs(n, start_vertex, current_path | singleton(vertex), vertex, adj, path_length -1, edges_found);
    }
    return;

}

void find_path_of_given_length(bitset* adj, size_t n, int path_length, bitset** already_found) {
    
    for(int i = 0; i < n; i++){
        find_path_recurs(n, i, singleton(i), i, adj, path_length, already_found);
    }
    return;
}

int readGraph(const char *graphString, struct graph *g) {
    g->numberOfVertices = getNumberOfVertices(graphString);
    if(g->numberOfVertices == -1 || g->numberOfVertices > MAXVERTICES) {
        return 1;
    }
    g->adjacencyList = malloc(sizeof(bitset)*g->numberOfVertices);
    if(loadGraph(graphString, g->numberOfVertices, g->adjacencyList) == -1) {
        return 1;
    }
    return 0;
}

bool isCounterExample(struct graph *g, char* interesting_criteria){
    bitset** edges_found = malloc(sizeof(bitset*)*g->numberOfVertices);
    for(int i = 0; i < g->numberOfVertices; i++){
        edges_found[i] = malloc(sizeof(bitset)*(i+1));
        for(int j = 0; j < i; j++){
            edges_found[i][j] = EMPTY;
        }
    }
    for(int i = g->numberOfVertices; i > 0; i--){
        find_path_of_given_length(g->adjacencyList, g->numberOfVertices, i, edges_found);
    }
    if(strcmp(interesting_criteria, "full") == 0){
    for(int i = 0; i < g->numberOfVertices; i++){
        for(int j = 0; j < i; j++){
            for (int k = 1; k < g->numberOfVertices; k++){
                if(intersection(edges_found[i][j],singleton(k)) == EMPTY){
                    fprintf(stderr,"no path between: ");
                            fprintf(stderr,"%d %d ",i,j);
                            fprintf(stderr," of length: ");
                            fprintf(stderr,"%d",k);
                            fprintf(stderr,"\n");
                }
            }
        }
    }
    
    for(int i = 0; i < g->numberOfVertices; i++){
        free(edges_found[i]);
    }
    free(g->adjacencyList);
    free(edges_found);
    return true;
    }
    
        for(int i = 0; i < g->numberOfVertices; i++){
        for(int j = 0; j < i; j++){
            for (int k = g->numberOfVertices/2; k < g->numberOfVertices; k++){
                if(intersection(edges_found[i][j],singleton(k)) == EMPTY){
                    for(int i = 0; i < g->numberOfVertices; i++){
                        free(edges_found[i]);
                    }
                    free(g->adjacencyList);
                    free(edges_found);
                    return true;
                }
            }
        }
    }
    
    for(int i = 0; i < g->numberOfVertices; i++){
        free(edges_found[i]);
    }
    free(g->adjacencyList);
    free(edges_found);
    return false;
    
}

int main(int argc, char ** argv) {
       
    unsigned long long int counter = 0;
    unsigned long long int passedGraphs = 0;
    clock_t start = clock();

    //  Start looping over lines of stdin.
    char * graphString = NULL;
    size_t size;
    while(getline(&graphString, &size, stdin) != -1) {
        struct graph g;
        readGraph(graphString, &g);
        counter++;
        bool passed = false;
        if(isCounterExample(&g,argv[1])) {
            passed = true;
        }
        if(passed) {
            passedGraphs++;
            printf("%s",graphString);
        }

    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    fprintf(stderr,"\rChecked %lld graphs in %f seconds: %llu passed.\n", counter, time_spent, passedGraphs);

    return 0;
}