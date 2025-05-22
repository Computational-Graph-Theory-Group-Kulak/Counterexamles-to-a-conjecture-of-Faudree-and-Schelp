#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include "readGraph/readGraph6.h"
/**
using u 128
*/
///This is the main function


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "bitset.h"

int compare_function(const void *a,const void *b) {
bitset *x = (bitset *) a;
bitset *y = (bitset *) b;
return size(*x) - size(*y);
}

bool find_cycle_recurs(bitset start_vertex, bitset current_path, size_t current_vertex, bitset* adj, int cycle_length) {
    if (cycle_length == 1){
        if((adj[current_vertex] & start_vertex) > 0){
            return true;
        }
        else{
            return false;
        }
    }
    bitset vertices_to_check = difference(adj[current_vertex],current_path);
    forEach(vertex, vertices_to_check) {
        if (find_cycle_recurs(start_vertex, current_path | singleton(vertex), vertex, adj, cycle_length -1)){
            return true;
        }
    }
    return false;

}

bool find_cycle_of_given_length(bitset* adj, int n, int cycle_length) {
    bool cycles_found = false;
    size_t i = 0;
    while( (i < n) & (!cycles_found)){
        cycles_found = find_cycle_recurs(singleton(i), singleton(i), i, adj, cycle_length);
        i++;
    }
    return cycles_found;
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

void lengths_of_head_section_recursive(bitset* adj, size_t n, size_t begin, size_t ending_chaining_point, bitset* path_lengths, bitset* path) {
    forEach(neighbour, intersection(adj[begin],complement(*path,n)) ){
        if (neighbour == ending_chaining_point){
            *path_lengths |= singleton(size(*path)+1);
            
        }
        else{

            *path |= singleton(neighbour);
            lengths_of_head_section_recursive(adj, n, neighbour, ending_chaining_point, path_lengths, path);
            *path &= complement(singleton(neighbour),n);
        }
    }
}

bitset lengths_of_head_section(bitset* adj, size_t n, size_t chaining_point_one, size_t ending_chaining_point) {
    bitset path = singleton(chaining_point_one);
    bitset path_lengths = EMPTY;
    lengths_of_head_section_recursive(adj, n, chaining_point_one, ending_chaining_point, &path_lengths, &path);
    return path_lengths;   
}

void lengths_of_middle_section_recursive(bitset* adj, size_t n, size_t chaining_point_one, size_t chaining_point_two, size_t chaining_point_three, size_t chaining_point_four, bitset* path_lengths, bitset* path, bool first_call) {
    
    if(first_call){
        forEach(neighbour, intersection(adj[chaining_point_one],complement(*path,n)) ){
            if (neighbour == chaining_point_two){
                *path |= singleton(neighbour);
                *path &= complement(singleton(chaining_point_four),n);
                lengths_of_middle_section_recursive(adj, n, chaining_point_one, chaining_point_two, chaining_point_three, chaining_point_four, path_lengths, path, false);
                *path &= complement(singleton(neighbour),n);
                *path |= singleton(chaining_point_four);
            }
            else{
                *path |= singleton(neighbour);
                lengths_of_middle_section_recursive(adj, n, neighbour, chaining_point_two, chaining_point_three, chaining_point_four, path_lengths, path, true);
                *path &= complement(singleton(neighbour),n);
            }
        }
    }else{
        forEach(neighbour, intersection(adj[chaining_point_three],complement(*path,n)) ){
            if (neighbour == chaining_point_four){
                *path_lengths |= singleton(size(*path)+1);
            }
            else{
                *path |= singleton(neighbour);
                lengths_of_middle_section_recursive(adj, n, chaining_point_one, chaining_point_two, neighbour, chaining_point_four, path_lengths, path, false);
                *path &= complement(singleton(neighbour),n);
            }
        }
    }
}

bitset lengths_of_middle_section(bitset* adj, size_t n, size_t chaining_point_one, size_t chaining_point_two, size_t chaining_point_three, size_t chaining_point_four) {
    bitset path = singleton(chaining_point_one) | singleton(chaining_point_three) | singleton(chaining_point_four);
    bitset path_lengths = EMPTY;
    lengths_of_middle_section_recursive(adj, n, chaining_point_one, chaining_point_two, chaining_point_three, chaining_point_four, &path_lengths, &path, true);
    return path_lengths;
}

size_t calculate_gcd(size_t a, size_t b) {
    if (b == 0) {
        return a;
    }
    return calculate_gcd(b, a % b);
}

bitset section_lengths_interact(size_t n, bitset head_lengths, bitset tail_lengths, bitset middle_lengths, int importance) {
    // Find the greatest common divisor of the middle lengths
    size_t gcd = 0;
    bool first_gcd = true;
    for (size_t i = 0; i < n+1; i++) {
        if ((middle_lengths & (1 << i)) != 0) {
            if (first_gcd) {
                gcd = i;
                first_gcd = false;
            } else {
                gcd = calculate_gcd(gcd, i);
            }
        }
    }
    if(gcd == 0){
        return false;
    }
    bitset possible_sums = EMPTY;
    // Check if all combinations of head and tail lengths sum to the same numbers modulo the gcd
    for (size_t i = 0; i < n+1; i++) {
        for (size_t j = 0; j < n+1; j++) {
            if ((head_lengths & (1 << i)) != 0) {
                if ((tail_lengths & (1 << j)) != 0) {
                    size_t sum = i+j;
                    possible_sums |= singleton(sum % gcd);
                }
            }
        }
    }

    if (gcd-size(possible_sums) >= importance){
        /**fprintf(stderr,"possible sums: ");
        for(int digit = 0; digit < n; digit++) {
            fprintf(stderr,"%c", possible_sums & (1 << digit) ? digit+'0' : 'n');
        }
        fprintf(stderr,"\n");
        fprintf(stderr," possible sums size: %i\n",size(possible_sums));
        fprintf(stderr," gcd+1-size(possible_sums): %lu\n",gcd-size(possible_sums));
        **/
        return gcd-size(possible_sums);
    }
    
    return EMPTY; // Interaction found
}



/**
 * A function to check if a hamiltonian connected graph has a spectrum gap of interest.
 * Returns which orientations have interesting spectrum gaps.
 * An orientation is represented by the vertex to which the first vertex is chained and the vertex that is chained to the same gadget.
 * The rest of the orientation is then fixed as no vertex can be chained to itself.
*/
size_t has_interesting_spectrum_gaps(bitset* adj, size_t n, size_t chaining_point_one, size_t chaining_point_two, size_t chaining_point_three, size_t chaining_point_four, size_t* interesting_orientations, int importance){
    size_t orientations_found = 0;
    bitset head_section_cycles = lengths_of_head_section(adj, n,  chaining_point_one, chaining_point_three);
    bitset tail_section_cycles = lengths_of_head_section(adj, n,  chaining_point_two, chaining_point_four);
    bitset middle_section_cycles = lengths_of_middle_section(adj, n, chaining_point_one, chaining_point_two, chaining_point_three, chaining_point_four);
    bitset spectrum_holes = section_lengths_interact(n,head_section_cycles, tail_section_cycles, middle_section_cycles, importance);
    if ( spectrum_holes != EMPTY){
        size_t gcd = 0;
        bool first_gcd = true;
        for (size_t i = 0; i < n+1; i++) {
            if ((middle_section_cycles & (1 << i)) != 0) {
                if (first_gcd) {
                    gcd = i;
                    first_gcd = false;
                } else {
                    gcd = calculate_gcd(gcd, i);
                }
            }
        }
        interesting_orientations[orientations_found*5] = chaining_point_two;
        interesting_orientations[orientations_found*5+1] = chaining_point_three;
        interesting_orientations[orientations_found*5+2] = chaining_point_four;
        interesting_orientations[orientations_found*5+3] = spectrum_holes;
        interesting_orientations[orientations_found*5+4] = gcd;
        orientations_found += 1;
    }
    head_section_cycles = lengths_of_head_section(adj, n,  chaining_point_one, chaining_point_four);
    tail_section_cycles = lengths_of_head_section(adj, n,  chaining_point_two, chaining_point_three);
    middle_section_cycles = lengths_of_middle_section(adj, n, chaining_point_one, chaining_point_two, chaining_point_four, chaining_point_three);
    spectrum_holes = section_lengths_interact(n,head_section_cycles, tail_section_cycles, middle_section_cycles, importance);
    if ( spectrum_holes != EMPTY){
        size_t gcd = 0;
        bool first_gcd = true;
        for (size_t i = 0; i < n+1; i++) {
            if ((middle_section_cycles & (1 << i)) != 0) {
                if (first_gcd) {
                    gcd = i;
                    first_gcd = false;
                } else {
                    gcd = calculate_gcd(gcd, i);
                }
            }
        }
        interesting_orientations[orientations_found*5] = chaining_point_two;
        interesting_orientations[orientations_found*5+1] = chaining_point_four;
        interesting_orientations[orientations_found*5+2] = chaining_point_three;
        interesting_orientations[orientations_found*5+3] = spectrum_holes;
        interesting_orientations[orientations_found*5+4] = gcd;
        orientations_found += 1;
    }
    head_section_cycles = lengths_of_head_section(adj, n,  chaining_point_one, chaining_point_two);
    tail_section_cycles = lengths_of_head_section(adj, n,  chaining_point_three, chaining_point_four);
    middle_section_cycles = lengths_of_middle_section(adj, n, chaining_point_one, chaining_point_three, chaining_point_two, chaining_point_four);
    spectrum_holes = section_lengths_interact(n,head_section_cycles, tail_section_cycles, middle_section_cycles, importance);
    if ( spectrum_holes != EMPTY){
        size_t gcd = 0;
        bool first_gcd = true;
        for (size_t i = 0; i < n+1; i++) {
            if ((middle_section_cycles & (1 << i)) != 0) {
                if (first_gcd) {
                    gcd = i;
                    first_gcd = false;
                } else {
                    gcd = calculate_gcd(gcd, i);
                }
            }
        }
        interesting_orientations[orientations_found*5] = chaining_point_three;
        interesting_orientations[orientations_found*5+1] = chaining_point_two;
        interesting_orientations[orientations_found*5+2] = chaining_point_four;
        interesting_orientations[orientations_found*5+3] = spectrum_holes;
        interesting_orientations[orientations_found*5+4] = gcd;
        orientations_found += 1;
    }
    head_section_cycles = lengths_of_head_section(adj, n,  chaining_point_one, chaining_point_four);
    tail_section_cycles = lengths_of_head_section(adj, n,  chaining_point_three, chaining_point_two);
    middle_section_cycles = lengths_of_middle_section(adj, n, chaining_point_one, chaining_point_three, chaining_point_four, chaining_point_two);
    spectrum_holes = section_lengths_interact(n,head_section_cycles, tail_section_cycles, middle_section_cycles, importance);
    if ( spectrum_holes != EMPTY){
        size_t gcd = 0;
        bool first_gcd = true;
        for (size_t i = 0; i < n+1; i++) {
            if ((middle_section_cycles & (1 << i)) != 0) {
                if (first_gcd) {
                    gcd = i;
                    first_gcd = false;
                } else {
                    gcd = calculate_gcd(gcd, i);
                }
            }
        }
        interesting_orientations[orientations_found*5] = chaining_point_three;
        interesting_orientations[orientations_found*5+1] = chaining_point_four;
        interesting_orientations[orientations_found*5+2] = chaining_point_two;
        interesting_orientations[orientations_found*5+3] = spectrum_holes;
        interesting_orientations[orientations_found*5+4] = gcd;
        orientations_found += 1;
    }
    head_section_cycles = lengths_of_head_section(adj, n,  chaining_point_one, chaining_point_two);
    tail_section_cycles = lengths_of_head_section(adj, n,  chaining_point_four, chaining_point_three);
    middle_section_cycles = lengths_of_middle_section(adj, n, chaining_point_one, chaining_point_four, chaining_point_two, chaining_point_three);

    spectrum_holes = section_lengths_interact(n,head_section_cycles, tail_section_cycles, middle_section_cycles, importance);
    if ( spectrum_holes != EMPTY){
        size_t gcd = 0;
        bool first_gcd = true;
        for (size_t i = 0; i < n+1; i++) {
            if ((middle_section_cycles & (1 << i)) != 0) {
                if (first_gcd) {
                    gcd = i;
                    first_gcd = false;
                } else {
                    gcd = calculate_gcd(gcd, i);
                }
            }
        }
        interesting_orientations[orientations_found*5] = chaining_point_four;
        interesting_orientations[orientations_found*5+1] = chaining_point_two;
        interesting_orientations[orientations_found*5+2] = chaining_point_three;
        interesting_orientations[orientations_found*5+3] = spectrum_holes;
        interesting_orientations[orientations_found*5+4] = gcd;
        orientations_found += 1;
    }
    
    head_section_cycles = lengths_of_head_section(adj, n,  chaining_point_one, chaining_point_three);
    tail_section_cycles = lengths_of_head_section(adj, n,  chaining_point_four, chaining_point_two);
    middle_section_cycles = lengths_of_middle_section(adj, n,  chaining_point_one, chaining_point_four, chaining_point_three, chaining_point_two);
    spectrum_holes = section_lengths_interact(n,head_section_cycles, tail_section_cycles, middle_section_cycles, importance);
    if ( spectrum_holes != EMPTY){
        size_t gcd = 0;
        bool first_gcd = true;
        for (size_t i = 0; i < n+1; i++) {
            if ((middle_section_cycles & (1 << i)) != 0) {
                if (first_gcd) {
                    gcd = i;
                    first_gcd = false;
                } else {
                    gcd = calculate_gcd(gcd, i);
                }
            }
        }
        interesting_orientations[orientations_found*5] = chaining_point_four;
        interesting_orientations[orientations_found*5+1] = chaining_point_three;
        interesting_orientations[orientations_found*5+2] = chaining_point_two;
        interesting_orientations[orientations_found*5+3] = spectrum_holes;
        interesting_orientations[orientations_found*5+4] = gcd;
        orientations_found += 1;
    }
    
    return orientations_found;
    
}

void check_path_recurs(bitset first_vertex, bitset* adj, bitset* path, bitset current_vertex, size_t n, bitset* end_points, bitset reachable_endpoints, size_t* counter, size_t* checked_via_degree, size_t* pruned_via_degree, size_t* cycles) {

    int current_vertex_integer = __builtin_ctzll(current_vertex);

    if( (adj[current_vertex_integer] & first_vertex) > 0) {
        cycles[size(*path)-1] += 1;
    }
    
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
            
            check_path_recurs(first_vertex, adj, path, vertex, n, end_points, reachable_endpoints, counter,checked_via_degree, pruned_via_degree, cycles);
            if(size(*end_points) == n){
                *path &= complement(current_vertex,n);
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
        if (size(adj[vertex]) < 3) {
            return false;
        }
    }
    return true;
}

bool check_ham_conn_graph6(const char* graph_str, size_t* counter, size_t* checked_via_degree, size_t* pruned_via_degree, char* filename, char* importance_character) {
    size_t n = getNumberOfVertices(graph_str);
    bitset *adj = malloc(sizeof(bitset)*n);
    size_t *interesting_orientations = malloc(sizeof(size_t)*5*6);
    *counter += 1;
    loadGraph(graph_str, n, adj);

    int importance;
    sscanf (importance_character,"%d",&importance);

    
    /**for(int row = 0; row <n; row++){
        for(int digit = 0; digit < n; digit++) {
            fprintf(stderr,"%c", adj[row] & (1 << digit) ? '1' : '0');
        }
        fprintf(stderr,"\n");
    }
    **/
    

   /**sort_vertices(adj, n);

    bitset *cycles = malloc(sizeof(size_t)*n);
    
    for(int i = 0; i < n; i++){
            cycles[i] = 0;
        }
    /**bitset* larger_adj = malloc(sizeof(bitset)*(n+1));**/
    for (size_t chaining_point_one = 0; chaining_point_one<n; chaining_point_one++){
        for (size_t chaining_point_two = chaining_point_one + 1; chaining_point_two<n; chaining_point_two++){
            for (size_t chaining_point_three =chaining_point_two+1; chaining_point_three<n; chaining_point_three++){
                for (size_t chaining_point_four = chaining_point_three+1; chaining_point_four<n; chaining_point_four++){
                    
                    /**for(size_t vertex = 0; vertex < n; vertex++){
                        larger_adj[vertex] = adj[vertex];
                    }
                    larger_adj[n+1] = singleton(chaining_point_one) | singleton(chaining_point_two) | singleton(chaining_point_three) | singleton(chaining_point_four);
                    larger_adj[chaining_point_one] |= singleton(n+1);
                    larger_adj[chaining_point_two] |= singleton(n+1);
                    larger_adj[chaining_point_three] |= singleton(n+1);
                    larger_adj[chaining_point_four] |= singleton(n+1);



                    for (size_t vertex = 0; vertex < n+1; ++vertex) {
                        bitset endpoints = ((1ULL << (vertex+1)) - 1);
                        bitset reachable_endpoints = complement(endpoints,n+1);
                        bitset path = singleton(vertex);
                        bitset current_vertex = singleton(vertex);
                        bitset first_vertex = current_vertex;
                        size_t special_point = 0;
                        check_path_recurs(first_vertex, larger_adj, &path, current_vertex, n+1, &endpoints, reachable_endpoints, counter, checked_via_degree, pruned_via_degree, cycles, &special_point);
                        if (size(endpoints) == n+1) {
                        
                            size_t* interesting_orientations = has_interesting_spectrum_gaps(adj, n, chaining_point_one, chaining_point_two, chaining_point_three, chaining_point_four);
                            if (interesting_orientations[0] != 0){
                                fptr = fopen(filename, "a");
                                fprintf(fptr,"%s", graph_str);
                                fprintf(fptr, "\n");
                                fclose(fptr);
                            }
                }
    }
    }}}}
    
    free(larger_adj);
    **/
   size_t number_of_interesting_orientations = has_interesting_spectrum_gaps(adj, n, chaining_point_one, chaining_point_two, chaining_point_three, chaining_point_four, interesting_orientations, importance);
    for (size_t i = 0; i < number_of_interesting_orientations; i++)
    {
        bitset* larger_adj = malloc(sizeof(bitset)*(n*2));
        for(size_t vertex = 0; vertex < n; vertex++){
                        larger_adj[vertex] = adj[vertex];
                        larger_adj[n+vertex] = adj[vertex]<<n;
                    }
        size_t chaining_point_left_bottom = interesting_orientations[5*i];
        size_t chaining_point_right_top = interesting_orientations[5*i+1];

        size_t chaining_point_right_bottom = interesting_orientations[5*i+2];

        larger_adj[chaining_point_left_bottom] |= singleton(chaining_point_one+n);
        larger_adj[chaining_point_one+n] |= singleton(chaining_point_left_bottom);
        larger_adj[chaining_point_one] |= singleton(chaining_point_left_bottom+n);
        larger_adj[chaining_point_left_bottom+n] |= singleton(chaining_point_one);
        larger_adj[chaining_point_right_top] |= singleton(chaining_point_right_bottom +n);
        larger_adj[chaining_point_right_bottom+n] |= singleton(chaining_point_right_top);
        larger_adj[chaining_point_right_top+n] |= singleton(chaining_point_right_bottom);
        larger_adj[chaining_point_right_bottom] |= singleton(chaining_point_right_top+n);
        /**fprintf(stderr," larger adj: \n");
        for(int row = 0; row <2*n; row++){
            for(int digit = 0; digit < 2*n; digit++) {
                fprintf(stderr,"%c", larger_adj[row] & (1 << digit) ? '1' : '0');
            }
            fprintf(stderr,"\n");
        }
        **/

        if (preliminary_checks(larger_adj, n*2)) {
            sort_vertices(larger_adj, 2*n);
            bitset *cycles = malloc(sizeof(bitset)*2*n);
            for(int i = 0; i < 2*n; i++){
                cycles[i] = 0;
            }
            bool is_ham_con = true;
            for (size_t vertex = 0; vertex < 2*n && is_ham_con; ++vertex) {
                        bitset endpoints = ((1ULL << (vertex+1)) - 1);
                        bitset reachable_endpoints = complement(endpoints,2*n+1);
                        bitset path = singleton(vertex);
                        bitset current_vertex = singleton(vertex);
                        bitset first_vertex = current_vertex;
                        check_path_recurs(first_vertex, larger_adj, &path, current_vertex, 2*n, &endpoints, reachable_endpoints, counter, checked_via_degree, pruned_via_degree, cycles);
                        if (size(endpoints) != 2*n) {
                            is_ham_con = false;
                        }
            }
            if (is_ham_con){
                FILE *fptr;
                fptr = fopen(filename, "a");
                fprintf(fptr,"%s", graph_str);
                fprintf(fptr, " %lu, %lu, %lu, %lu \n expected holes in cycles: ", chaining_point_one, chaining_point_two, chaining_point_three, chaining_point_four);
                fprintf(fptr, " %lu every %lu lengths", interesting_orientations[5*i+3],interesting_orientations[5*i+4]);
                
                fprintf(fptr, "\n");
                fclose(fptr);
            }
            free(cycles);
        }

        free(larger_adj);
    }
    
                }}} }
    
    free(adj);
    free(interesting_orientations);
    /**free(cycles);**/
    return false;
    
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

            if (check_ham_conn_graph6(line, &stepcounter, &checked_via_degree, &pruned_via_degree, argv[1],argv[2])){
             
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
                else if (check_ham_conn_graph6(line, &stepcounter, &checked_via_degree, &pruned_via_degree, argv[1], argv[2])){
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
    


