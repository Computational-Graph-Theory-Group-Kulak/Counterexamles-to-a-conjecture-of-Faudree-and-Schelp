#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include "readGraph/readGraph6.h"
#include "bitset.h"
/**
 * hamcon_paths.c
 * 
 * Author: Michiel provoost (michiel.provoost@kuleuven.be)
 *
 */



#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

int compare_function(const void *a,const void *b) {
bitset *x = (bitset *) a;
bitset *y = (bitset *) b;
return size(*x) - size(*y);
}

void find_path_recurs(size_t n, bitset start_vertex, bitset current_path, size_t current_vertex, bitset* adj, int path_length, bitset** edges_found) {

    if (path_length == 1){
        bitset vertices_to_connect= difference(adj[current_vertex],current_path);
        forEach(vertex, vertices_to_connect) {
            if(vertex  < __builtin_ctzll(start_vertex)){
                edges_found[__builtin_ctzll(start_vertex)][vertex] |= singleton(size(current_path));
            }else{
                edges_found[vertex][__builtin_ctzll(start_vertex)] |= singleton(size(current_path));
            }
        }

        return;
    }
    bitset vertices_to_check = difference(adj[current_vertex],current_path);
    forEach(vertex, vertices_to_check) {
        find_path_recurs(n, start_vertex, current_path | singleton(vertex), vertex, adj, path_length -1, edges_found);
    }
    return;

}

void find_path_of_given_length(bitset* adj, size_t n, int cycle_length, bitset** already_found) {
    
    for(int i = 0; i < n; i++){
        find_path_recurs(n, singleton(i), singleton(i), i, adj, cycle_length, already_found);
    }
    return;
}

int determine_vertex_options(bitset* adj, bitset* path, bitset* vertices_to_check, bitset* endpoints, bitset* reachable_endpoints, size_t n, bitset* vertex_options, size_t* checked_via_degree, size_t* pruned_via_degree) {
    int found_required_edge = 0;
    bitset* required_edges = calloc(2,sizeof(bitset));
    bitset neighbours;
    bitset bitset_vertex = EMPTY;
    int i = 0;
    bitset path_complement = complement(*path,n);
    bitset full_path = (path_complement & (path_complement-1));
    bool required_edges_check = true;
    
    forEach(vertex, *vertices_to_check) {
        
        bitset_vertex = singleton(vertex);
        if(full_path == 0){
            vertex_options[i] = bitset_vertex;
            i++;
        }else{
            if(required_edges_check){
                *checked_via_degree += 1;
                neighbours = difference(adj[vertex],*path);
                if ((neighbours & (neighbours - 1)) == 0) {
                    found_required_edge++;
                
                    /**fprintf(stderr,"vertices to check before filtering:");
                    for(int digit = 0; digit < n; digit++) {
                        fprintf(stderr,"%c", *vertices_to_check & (1 << digit) ? '1' : '0');
                        }
                    fprintf(stderr,"\t");
                    fprintf(stderr,"path: ");
                    for(int digit = 0; digit < n; digit++) {
                        fprintf(stderr,"%c", *path & (1 << digit) ? '1' : '0');
                        }
                    fprintf(stderr,"\n");
                    */
                    if (found_required_edge == 1) {
                        required_edges[found_required_edge-1] = bitset_vertex;
                        } 
                    else {
                        if(found_required_edge == 2){
                            if(intersection(required_edges[0],*endpoints) != EMPTY){
                                if (intersection(bitset_vertex,*reachable_endpoints) !=EMPTY){
                                    vertex_options[0] = required_edges[0];
                                    free(required_edges);
                                    *pruned_via_degree += size(*vertices_to_check) - 1;
                                    return 1;
                                    }
                                else{
                                    free(required_edges);
                                    *pruned_via_degree += size(*vertices_to_check);
                                    return 0;
                                    }
                            
                                }
                            else{
                                if(intersection(bitset_vertex,*endpoints) != EMPTY){
                                    if (intersection(required_edges[0],*reachable_endpoints) !=EMPTY){
                                        vertex_options[0] = bitset_vertex;
                                        free(required_edges);
                                        *pruned_via_degree += size(*vertices_to_check) - 1;
                                        return 1;
                                        }
                                    else{
                                        free(required_edges);
                                        *pruned_via_degree += size(*vertices_to_check);
                                        return 0;
                                        }
                                
                                    }
                                else{
                                    required_edges[found_required_edge-1] = bitset_vertex;
                                    }
                                }
                            }
                        else{
                            free(required_edges);
                            *pruned_via_degree += size(*vertices_to_check);
                            return 0;
                            }
                    
                        }
                    }
                }   
            if(neighbours != EMPTY){
                    vertex_options[i] = bitset_vertex;
                    int j = 0;
                    while (j < i && size(adj[__builtin_ctzll(vertex_options[j])] & ~*path) <= size(adj[vertex]& ~*path)) {
                        j++;
                    }
                    i++;
            }
        }
    }
            
        
    
    /**
    fprintf(stderr,"vertices to check after filtering:");
    for(int digit = 0; digit < n; digit++) {
        fprintf(stderr,"%lu", vertex_options[digit]);
    }
    fprintf(stderr,"\n");
    */
    if(found_required_edge == 2){
        vertex_options[0] = required_edges[0];
        vertex_options[1] = required_edges[1];
        *reachable_endpoints = required_edges[0];
        *reachable_endpoints |= required_edges[1];
        free(required_edges);
        *pruned_via_degree += 1;
        return 2;
        
    }
    
    if(found_required_edge == 1 && (intersection(*reachable_endpoints,required_edges[0])== EMPTY || intersection(required_edges[0],*endpoints) != EMPTY)){
        vertex_options[0] = required_edges[0];
        free(required_edges);
        return 1;

    }
    
    free(required_edges);
    return i;
}

void sort_vertices(uint64_t* adj, size_t n){

    size_t *sorted_adj = malloc(sizeof(size_t)*n);    
    bitset *new_adj = malloc(sizeof(bitset)*n);
    new_adj[0] = EMPTY;
    sorted_adj[0] = 0;
    for (size_t vertex = 1; vertex < n; vertex++) {
        
            new_adj[vertex] = EMPTY;
            size_t i = 0;
            size_t new_degree = size(adj[vertex]);
            while (i < vertex-1 && new_degree <= size(adj[sorted_adj[i]])){
                i++;

            }
            
            for(int j = vertex; j > i; j--){
                sorted_adj[j] = sorted_adj[j-1];
            }
            sorted_adj[i] = vertex;
            
        }
    for (size_t i = 0; i < n; i++) {
        size_t vertex = sorted_adj[i];
        for (size_t j = 0; j < n; j++) {
            size_t neighbour = sorted_adj[j];
            if (vertex != neighbour && intersection(adj[vertex], singleton(neighbour)) != 0) {
                new_adj[i] |=singleton(j);
                new_adj[j] |=singleton(i);
            }
        }
    }
    for (size_t vertex = 0; vertex < n; vertex++) {
        adj[vertex] = new_adj[vertex];
    }
    free(sorted_adj);
    free(new_adj);
}

/**
void check_path(uint64_t* adj, bitset* path, size_t current_vertex, size_t n, uint64_t* end_points) {
    size_t i = 0;
    size_t path_vertex_stack[n];
    bitset vertex_to_check_stack[n];
    for (size_t j = 0; j < n; ++j) {
        path_vertex_stack[j] = 0ULL;
        vertex_to_check_stack[j] = EMPTY;
    }
    size_t vertices_to_check_unfiltered = difference(adj[current_vertex],*path);
    bitset* vertices_to_check = determine_vertex_options(adj, path, &vertices_to_check_unfiltered, end_points, n);
    while (size(*end_points) != n &&  (i !=0 || (vertices_to_check !=EMPTY && size(intersection(complement(*path,n),complement(*end_points,n))) != 0))) {
        if (size(*path) == n) {
            add(*end_points,current_vertex);
            removeElement(*path,current_vertex);
            current_vertex = path_vertex_stack[--i];
            vertices_to_check = vertex_to_check_stack[i];
            continue;
        }
        if (vertices_to_check == EMPTY) {
            removeElement(*path,current_vertex);
            current_vertex = path_vertex_stack[--i];
            vertices_to_check = vertex_to_check_stack[i];
            continue;
        }
        if (size(intersection(complement(*path,n),complement(*end_points,n))) == 0 && i > 0) {
            removeElement(*path,current_vertex);
            current_vertex = path_vertex_stack[--i];
            vertices_to_check = vertex_to_check_stack[i];
            continue;
        }
        if(vertices_to_check != EMPTY && size(intersection(complement(*path,n),complement(*end_points,n))) != 0){
            path_vertex_stack[i] = current_vertex;
            current_vertex = __builtin_ctz(vertices_to_check);
            
            fprintf(stderr,"current vertex: %zu\n",current_vertex);
            removeElement(vertices_to_check,current_vertex);
            vertex_to_check_stack[i] = vertices_to_check;
            i+=1;
            add(*path,current_vertex);
            vertices_to_check = difference(adj[current_vertex],*path);
            vertices_to_check = determine_vertex_options(adj, path, &vertices_to_check, end_points, n);
             
           
        }
        
    }
}
*/

void check_path_recurs(bitset first_vertex, bitset* adj, bitset* path, bool** path_matrix, bitset current_vertex, size_t n, bitset* end_points, bitset reachable_endpoints, size_t* counter, size_t* checked_via_degree, size_t* pruned_via_degree, bitset** cycles) {
    *counter += 1;
    int current_vertex_integer = __builtin_ctzll(current_vertex);
    
    if (complement(*path,n) == EMPTY) {
        *end_points  |= current_vertex;
        /**
        fprintf(stderr,"endpoints: ");
                for(int digit = 0; digit < n; digit++) {
                    fprintf(stderr,"%c", *end_points & (1 << digit) ? '1' : '0');
                }
                fprintf(stderr,"\n");
                */
        *path &= complement(current_vertex,n);
        return;
    }

    if (complement(union(*path,*end_points),n) == EMPTY) {
            *path &= complement(current_vertex,n);
            return;
        }
    
    /**fprintf(stderr,"vertex: %i",current_vertex_integer);
     * */
    

    bitset vertices_to_check = difference(adj[current_vertex_integer],*path);
    bitset* neighbours = malloc(n*sizeof(bitset));
    forEach(neighbour, vertices_to_check){
        if(__builtin_ctzll(first_vertex) > neighbour){
            cycles[__builtin_ctzll(first_vertex)][neighbour] |= singleton(size(*path)-1);

        }
        else{
            cycles[neighbour][__builtin_ctzll(first_vertex)] |= singleton(size(*path)-1);
        }

    }
    int nb_of_options;
    nb_of_options = determine_vertex_options(adj, path, &vertices_to_check, end_points, &reachable_endpoints, n, neighbours, checked_via_degree, pruned_via_degree);
    bitset vertex;
    /**
    fprintf(stderr,"nb of options: %i\n",nb_of_options);
    fprintf(stderr, "neighbours: ");
    for(int digit = 0; digit < nb_of_options; digit++) {
        fprintf(stderr,"%lu ", neighbours[digit]);
    }
    fprintf(stderr,"\n");
    */
    for (int i = 0; i < nb_of_options; i++){
            vertex = neighbours[i];
            *path |= vertex;
            path_matrix[current_vertex_integer][__builtin_ctzll(vertex)] = true;
            path_matrix[__builtin_ctzll(vertex)][current_vertex_integer] = true;
            check_path_recurs(first_vertex, adj, path, path_matrix, vertex, n, end_points, reachable_endpoints, counter,checked_via_degree, pruned_via_degree, cycles);
            path_matrix[current_vertex_integer][__builtin_ctzll(vertex)] = false;
            path_matrix[__builtin_ctzll(vertex)][current_vertex_integer] = false;
            if(size(*end_points) == n){
                *path &= ~current_vertex;
                free(neighbours);
                return;
            }
        /**
        fprintf(stderr,"path: ");
                for(int digit = 0; digit < n; digit++) {
                    fprintf(stderr,"%c", *path & (1 << digit) ? '1' : '0');
                }
                fprintf(stderr,"\n");
        */
    }
    *path &= complement(current_vertex,n);
    free(neighbours);
    return;
}

    

bool preliminary_checks(uint64_t* adj, size_t n) {
    for (size_t vertex = 0; vertex < n; ++vertex) {
        if (__builtin_popcountll(adj[vertex]) < 2) {
            return false;
        }
    }
    return true;
}

bool check_ham_conn_graph6(const char* graph_str, size_t* counter, size_t* checked_via_degree, size_t* pruned_via_degree, char* interesting_criteria) {
    size_t n = getNumberOfVertices(graph_str);
    bitset *adj = malloc(sizeof(bitset)*n);
    *counter += 1;
    loadGraph(graph_str, n, adj);

    if (!preliminary_checks(adj, n)) {
        free(adj);
        return false;
    }
    /**for(int row = 0; row <n; row++){
        for(int digit = 0; digit < n; digit++) {
            fprintf(stderr,"%c", adj[row] & (1 << digit) ? '1' : '0');
        }
        fprintf(stderr,"\n");
    }
    */

    //sort_vertices(adj, n);
    //This is the array of bitsets that will store the cycle lengths found for each edge
    //Cycles[i][j] will store the cycle length found for the edge between vertex i and vertex j
    bitset** cycles = (bitset**)malloc(n * sizeof(bitset*));
    for (int i = 0; i < n; i++){
        cycles[i] = (bitset*)malloc((i+1) * sizeof(bitset));
        for(int j = 0; j < i; j++){
            cycles[i][j] = EMPTY;
        }
    }
    for (size_t vertex = 0; vertex < n; ++vertex) {
        bitset endpoints = ((1ULL << (vertex+1)) - 1);
        bitset reachable_endpoints = complement(endpoints,n);
        bitset path = singleton(vertex);
        bool** path_matrix = malloc(n * sizeof(bool*));
        for (int i = 0; i < n; i++){
            path_matrix[i] = malloc(n * sizeof(bool));
            for(int j = 0; j< n; j++){
                path_matrix[i][j] = false;
            }
        }
        bitset current_vertex = singleton(vertex);
        bitset first_vertex = current_vertex;
        check_path_recurs(first_vertex, adj, &path, path_matrix, current_vertex, n, &endpoints, reachable_endpoints, counter, checked_via_degree, pruned_via_degree, cycles);
        for(int i = 0; i < n; i++){
            free(path_matrix[i]);
        }
        free(path_matrix);
        if (size(endpoints) != n) {
            free(adj);
            for(int i = 0; i < n; i++){
                free(cycles[i]);
            }
            free(cycles);
            return false;
        }
    }

    bool is_interesting = false;
    if(strcmp(interesting_criteria, "any") == 0){
        is_interesting = true;
    
    }
    
    if(strcmp(interesting_criteria, "periodic") == 0){

        for(int i = 2; i < n; i++){
            find_path_of_given_length(adj,n,i, cycles);
        }
            
        is_interesting = false;
        bool was_uninteresting = true;
        for(int i = 0; i < n; i++){
            forEach(j, adj[i]){
                if(cycles[i][j] != (1<<n)-1){
                
                    if (!was_uninteresting){
                        is_interesting = true;
                    }
            
                    else{
                        was_uninteresting = false;
                    }
                }   
            }
        }
        /**if (is_interesting){
            fptr = fopen(filename, "a");
            for(int i = 0; i < n; i++){
                forEach(j, adj[i]){
                    for(int k = 0; k < n-1; k++){
                        fprintf(fptr, "%d %d %c",i,j, cycles[i][j] & (1 << k) ? '1' : '0');
                    }
                    fprintf(fptr," ");
                }
            }
            fprintf(fptr,"%s", graph_str);
            fprintf(fptr, "\n");
            fclose(fptr);
        }
        **/
    }
    if(strcmp(interesting_criteria, "last") == 0){
         for (int i = 0; i < n; i++){
            for(int j = 0; j < i; j++){
                cycles[i][j] = EMPTY;
            }
        }
        
        for(int i = (n/2)-1; i < n; i++){
            find_path_of_given_length(adj,n,i, cycles);
        }
    
        
    
        is_interesting = false;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < i; j++){
                    for(int k = (n+1)/2;k<n;k++){
                        if(intersection(cycles[i][j], singleton(k)) == 0){
                            is_interesting = true;
                            fprintf(stderr,"no path between: ");
                            fprintf(stderr,"%d %d ",i,j);
                            fprintf(stderr," of length: ");
                            fprintf(stderr,"%d",k);
                            fprintf(stderr,"\n");
                        }
                    }
                
                     
            }
        }
        }
        if(strcmp(interesting_criteria, "all") == 0){
         for (int i = 0; i < n; i++){
            for(int j = 0; j < i; j++){
                cycles[i][j] = EMPTY;
            }
        }
        
        for(int i = n; i > 0; i--){
            find_path_of_given_length(adj,n,i, cycles);
        }
    
        
    
        is_interesting = false;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < i; j++){
                    for(int k = 2;k<n;k++){
                        if(intersection(cycles[i][j], singleton(k)) == 0){
                            is_interesting = true;
                            fprintf(stderr,"no path between: ");
                            fprintf(stderr,"%d %d ",i,j);
                            fprintf(stderr," of length: ");
                            fprintf(stderr,"%d",k);
                            fprintf(stderr,"\n");
                        }
                    }
                
            }
        }
        }

        if(strcmp(interesting_criteria, "full") == 0){
         for (int i = 0; i < n; i++){
            for(int j = 0; j < i; j++){
                cycles[i][j] = EMPTY;
            }
        }
        
        for(int i = n; i > 0; i--){
            find_path_of_given_length(adj,n,i, cycles);
        }
    
        
    
        is_interesting = true;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < i; j++){
                    for(int k = 2;k<n;k++){
                        if(intersection(cycles[i][j], singleton(k)) == 0){
                            is_interesting = false;
                        }
                    }
                
            }
        }
        }
    free(adj);
    for(int i = 0; i < n; i++){
        free(cycles[i]);
    }
    free(cycles);
    if(is_interesting){
        fprintf(stderr,"new graph\n");
    }
    return is_interesting;
}

int main(int argc, char ** argv) {
    size_t stepcounter = 0;
    size_t pruned_via_degree = 0;
    size_t checked_via_degree = 0;
    bool gengy = false;
    if (gengy == false){
        char *line = NULL;
        unsigned long long int counter = 0;
        
        unsigned long long int passedGraphs = 0;
        clock_t start = clock();
        size_t size;
        while(getline(&line, &size, stdin) != -1)
        {
            counter++;

            if (check_ham_conn_graph6(line, &stepcounter, &checked_via_degree, &pruned_via_degree, argv[1])){
             
                printf("%s",line);
                passedGraphs++;
            }
        }
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(stderr,"\rChecked %lld graphs in %f seconds: %llu passed. Needed %lu steps. Pruned %f options with degree.\n", counter, time_spent, passedGraphs, stepcounter, ((float)pruned_via_degree)/(float)checked_via_degree);
    }else{
        
        char* subgraph = "";
        char* line = NULL;
        unsigned long long int counter = 0;
        unsigned long long int passedGraphs = 0;
        // Consumes the iterator, returns an (Optional) String
        size_t size;
        clock_t start = clock();
        while(getline(&line, &size, stdin) != -1){
            counter++;
            
            bool is_subgraph = strlen(subgraph) == strlen(line);
                for (int i = 0; i < strlen(subgraph);i++){
                    if (subgraph[i] != line[i]){
                        is_subgraph = false;
                    
                    }
                }
                if (is_subgraph){
                    printf("%s",line);
                    passedGraphs++;
                }
                else if (check_ham_conn_graph6(line, &stepcounter, &checked_via_degree, &pruned_via_degree, argv[1])){
                        printf("%s",line);
                        subgraph = line;
                        passedGraphs++;
                        
                    }
                } 
                
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
                fprintf(stderr,"\rChecked %lld graphs in %f seconds: %llu passed. Needed %lu steps. Pruned %lu options with degree. \n", counter, time_spent, passedGraphs, stepcounter, pruned_via_degree);
                
            }
       return 0; 
    }
    


