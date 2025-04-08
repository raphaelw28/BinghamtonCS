#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


void directMapped(FILE* input_file, FILE* output_file){
    int cacheSizes[] = {1024, 4096, 16384, 32768};
    int lineSize = 32;
    int numConfigs = 4;
    int maxLines = 32768 / 32;

    int hits[numConfigs];
    int accesses[numConfigs];
    unsigned int caches[numConfigs][maxLines];

    for(int i = 0; i < numConfigs; i++){
        hits[i] = 0;
        accesses[i] = 0;
        for(int j = 0; j < maxLines; j++){
            caches[i][j] = 0;
        }
    }

    char type;
    unsigned int addr;
    while(fscanf(input_file, " %c %x", &type, &addr) == 2){
        for(int i = 0; i < numConfigs; i++){
            int numLines = cacheSizes[i] / lineSize;
            int index = (addr / lineSize) % numLines;
            unsigned int tag = addr / cacheSizes[i];

            accesses[i]++;
            if(caches[i][index] == tag){
                hits[i]++;
            } else {
                caches[i][index] = tag;
            }
        }
    }

    for(int i = 0; i < numConfigs; i++){
        fprintf(output_file, "%d,%d ", hits[i], accesses[i]);
    }
    fprintf(output_file, "\n");

}

void setAssociative(FILE* input_file, FILE* output_file){
    int associativities[] = {2,4,8,16};
    int hits[4] = {0};
    int accesses[4] = {0};

    for(int config = 0; config < 4; config++){
        int associativity = associativities[config];
        int numSets = 16384 / (32 * associativity);

        unsigned int cache[numSets][associativity];
        int lru[numSets][associativity];

        for(int i = 0; i < numSets; i++){
            for(int j = 0; j < associativity; j++){
                cache[i][j] = 0;
                lru[i][j] = -1;
            }
        }

        char type;
        unsigned int addr;
        int timestamp = 0;

        while(fscanf(input_file, " %c %x", &type, &addr) == 2){
            accesses[config]++;
            int setIndex = (addr / 32) % numSets;
            unsigned int tag = addr / (16384 / associativity);

            int hit = 0;
            int oldest = 0;

            for(int i = 0; i < associativity; i++){
                if(cache[setIndex][i] == tag){
                    hit = 1;
                    lru[setIndex][i] = timestamp;
                    break;
                }
                if(lru[setIndex][i] < lru[setIndex][oldest]){
                    oldest = i;
                }
            }
            if(hit){
                hits[config]++;
            } else {
                cache[setIndex][oldest] = tag;
                lru[setIndex][oldest] = timestamp;
            }
            timestamp++;
        }
        rewind(input_file);
    }
    for (int i = 0; i < 4; i++) {
        fprintf(output_file, "%d,%d ", hits[i], accesses[i]);
    }
    fprintf(output_file, "\n");
}

void fullyAssociative(FILE* input_file, FILE* output_file){
    const int cache_size = 512;
    const int total_nodes = 2 * cache_size - 1; 
    const int internal_nodes = total_nodes - cache_size; 

    unsigned int cache_lru[cache_size];
    int lru[cache_size];

    unsigned int cache_hc[cache_size];
    int hot_cold[internal_nodes];

    for (int i = 0; i < cache_size; i++) {
        cache_lru[i] = 0;
        lru[i] = -1;
        cache_hc[i] = 0;
    }
    for (int i = 0; i < internal_nodes; i++) {
        hot_cold[i] = 0; 
    }

    int hits_lru = 0, accesses_lru = 0;
    int hits_hc = 0, accesses_hc = 0;
    int timestamp = 0;

    char type;
    unsigned int addr;

    while (fscanf(input_file, " %c %x", &type, &addr) == 2) {
        unsigned int tag = addr / 32; 

        accesses_lru++;
        int hit = 0, oldest_index = 0, oldest_time = timestamp;
        for (int i = 0; i < cache_size; i++) {
            if (cache_lru[i] == tag) {
                hits_lru++;
                lru[i] = timestamp;
                hit = 1;
                break;
            }
            if (lru[i] < oldest_time) {
                oldest_time = lru[i];
                oldest_index = i;
            }
        }
        if (!hit) {
            cache_lru[oldest_index] = tag;
            lru[oldest_index] = timestamp;
        }

        accesses_hc++;
        hit = 0;
        int cache_line_index = -1;
        for (int i = 0; i < cache_size; i++) {
            if (cache_hc[i] == tag) {
                hits_hc++;
                hit = 1;
                cache_line_index = i;
                break;
            }
        }

        if (hit) {
            int node = cache_line_index + internal_nodes; 
            while (node > 0) {
                int parent = (node - 1) / 2;
                hot_cold[parent] = (node % 2 == 1) ? 0 : 1;
                node = parent;
            }
        } else {
            int node = 0; 
            while (node < internal_nodes) {
                int next_node;
                if (hot_cold[node] == 0) {
                    next_node = 2 * node + 2; 
                } else {
                    next_node = 2 * node + 1; 
                }
                hot_cold[node] = (next_node % 2 == 1) ? 0 : 1;
                node = next_node;
            }
            int replace_index = node - internal_nodes;
            cache_hc[replace_index] = tag;

            node = replace_index + internal_nodes;
            while (node > 0) {
                int parent = (node - 1) / 2;
                hot_cold[parent] = (node % 2 == 1) ? 0 : 1;
                node = parent;
            }
        }

        timestamp++;
    }

    fprintf(output_file, "%d,%d ", hits_lru, accesses_lru);
    fprintf(output_file, "%d,%d\n", hits_hc, accesses_hc);
}

void twoLevelCache(FILE* input_file, FILE* output_file) {
    int l1_line_size = 32;  
    int l2_line_size = 64;   

    int l1_hits = 0, l1_accesses = 0;
    int l2_hits = 0, l2_accesses = 0;

    int l1_cache_size = 4 * 1024;       
    int l1_associativity = 4;         
    int l1_num_sets = l1_cache_size / (l1_line_size * l1_associativity);

    int l2_cache_size = 64 * 1024;       
    int l2_associativity = 8;            
    int l2_num_sets = l2_cache_size / (l2_line_size * l2_associativity);

    unsigned int **l1_tags = malloc(l1_num_sets * sizeof(unsigned int *));
    int **l1_lru = malloc(l1_num_sets * sizeof(int *));
    int **l1_valid = malloc(l1_num_sets * sizeof(int *));
    for (int i = 0; i < l1_num_sets; i++) {
        l1_tags[i] = calloc(l1_associativity, sizeof(unsigned int));
        l1_lru[i] = calloc(l1_associativity, sizeof(int));
        l1_valid[i] = calloc(l1_associativity, sizeof(int));
    }

    unsigned int **l2_tags = malloc(l2_num_sets * sizeof(unsigned int *));
    int **l2_lru = malloc(l2_num_sets * sizeof(int *));
    int **l2_valid = malloc(l2_num_sets * sizeof(int *));
    for (int i = 0; i < l2_num_sets; i++) {
        l2_tags[i] = calloc(l2_associativity, sizeof(unsigned int));
        l2_lru[i] = calloc(l2_associativity, sizeof(int));
        l2_valid[i] = calloc(l2_associativity, sizeof(int));
    }

    char operation;
    unsigned int address;
    while (fscanf(input_file, " %c %x", &operation, &address) == 2) {
        if (operation >= 'a' && operation <= 'z') {
            operation -= 'a' - 'A';
        }

        l1_accesses++;  

        unsigned int block_address = address / l1_line_size;
        unsigned int l1_set_index = block_address % l1_num_sets;
        unsigned int l1_tag = block_address / l1_num_sets;

        int hit_l1 = 0;
        int lru_index_l1 = -1;
        int max_lru_l1 = -1;
        int found_invalid = 0;

        for (int i = 0; i < l1_associativity; i++) {
            if (l1_valid[l1_set_index][i] && l1_tags[l1_set_index][i] == l1_tag) {
                l1_hits++;
                hit_l1 = 1;
                l1_lru[l1_set_index][i] = 0;  
                break;
            } else {
                if (!l1_valid[l1_set_index][i] && !found_invalid) {
                    lru_index_l1 = i;
                    found_invalid = 1;
                } else if (!found_invalid && l1_lru[l1_set_index][i] > max_lru_l1) {
                    max_lru_l1 = l1_lru[l1_set_index][i];
                    lru_index_l1 = i;
                }
                if (l1_valid[l1_set_index][i]) {
                    l1_lru[l1_set_index][i]++;
                }
            }
        }

        int l2_access_needed = 0;
        if (operation == 'S' || operation == 'W') {
            l2_access_needed = 1; 
        } else if (!hit_l1) {
            l2_access_needed = 1;
        }

        if (l2_access_needed) {
            l2_accesses++;  

            unsigned int block_address_l2 = address / l2_line_size;
            unsigned int l2_set_index = block_address_l2 % l2_num_sets;
            unsigned int l2_tag = block_address_l2 / l2_num_sets;

            int hit_l2 = 0;
            int lru_index_l2 = -1;
            int max_lru_l2 = -1;
            int found_invalid_l2 = 0;

            for (int i = 0; i < l2_associativity; i++) {
                if (l2_valid[l2_set_index][i] && l2_tags[l2_set_index][i] == l2_tag) {
                    l2_hits++;  
                    hit_l2 = 1;
                    l2_lru[l2_set_index][i] = 0;  
                    break;
                } else {
                    if (!l2_valid[l2_set_index][i] && !found_invalid_l2) {
                        lru_index_l2 = i;
                        found_invalid_l2 = 1;
                    } else if (!found_invalid_l2 && l2_lru[l2_set_index][i] > max_lru_l2) {
                        max_lru_l2 = l2_lru[l2_set_index][i];
                        lru_index_l2 = i;
                    }
                    if (l2_valid[l2_set_index][i]) {
                        l2_lru[l2_set_index][i]++;
                    }
                }
            }

            if (!hit_l2) {
                l2_tags[l2_set_index][lru_index_l2] = l2_tag;
                l2_valid[l2_set_index][lru_index_l2] = 1;
                l2_lru[l2_set_index][lru_index_l2] = 0;
            }
        }

        if (!hit_l1) {
            l1_tags[l1_set_index][lru_index_l1] = l1_tag;
            l1_valid[l1_set_index][lru_index_l1] = 1;
            l1_lru[l1_set_index][lru_index_l1] = 0;
        }
    }

    for (int i = 0; i < l1_num_sets; i++) {
        free(l1_tags[i]);
        free(l1_lru[i]);
        free(l1_valid[i]);
    }
    free(l1_tags);
    free(l1_lru);
    free(l1_valid);

    for (int i = 0; i < l2_num_sets; i++) {
        free(l2_tags[i]);
        free(l2_lru[i]);
        free(l2_valid[i]);
    }
    free(l2_tags);
    free(l2_lru);
    free(l2_valid);

    fprintf(output_file, "%d,%d;%d,%d\n", l1_hits, l1_accesses, l2_hits, l2_accesses);
}

void twoLevelCacheWriteBack(FILE* input_file, FILE* output_file) {
    int l1_line_size = 32;  
    int l2_line_size = 64;  

    int l1_hits = 0, l1_accesses = 0;
    int l2_hits = 0, l2_accesses = 0;

    int l1_cache_size = 4 * 1024;     
    int l1_associativity = 4;            
    int l1_num_sets = l1_cache_size / (l1_line_size * l1_associativity);

    int l2_cache_size = 64 * 1024;       
    int l2_associativity = 8;          
    int l2_num_sets = l2_cache_size / (l2_line_size * l2_associativity);

    unsigned int **l1_tags = malloc(l1_num_sets * sizeof(unsigned int *));
    int **l1_lru = malloc(l1_num_sets * sizeof(int *));
    int **l1_valid = malloc(l1_num_sets * sizeof(int *));
    int **l1_dirty = malloc(l1_num_sets * sizeof(int *));  
    for (int i = 0; i < l1_num_sets; i++) {
        l1_tags[i] = calloc(l1_associativity, sizeof(unsigned int));
        l1_lru[i] = calloc(l1_associativity, sizeof(int));
        l1_valid[i] = calloc(l1_associativity, sizeof(int));
        l1_dirty[i] = calloc(l1_associativity, sizeof(int));
    }

    unsigned int **l2_tags = malloc(l2_num_sets * sizeof(unsigned int *));
    int **l2_lru = malloc(l2_num_sets * sizeof(int *));
    int **l2_valid = malloc(l2_num_sets * sizeof(int *));
    int **l2_dirty = malloc(l2_num_sets * sizeof(int *));  
    for (int i = 0; i < l2_num_sets; i++) {
        l2_tags[i] = calloc(l2_associativity, sizeof(unsigned int));
        l2_lru[i] = calloc(l2_associativity, sizeof(int));
        l2_valid[i] = calloc(l2_associativity, sizeof(int));
        l2_dirty[i] = calloc(l2_associativity, sizeof(int));
    }

    char operation;
    unsigned int address;
    while (fscanf(input_file, " %c %x", &operation, &address) == 2) {
        if (operation >= 'a' && operation <= 'z') {
            operation -= 'a' - 'A';
        }

        l1_accesses++;  

        unsigned int block_address = address / l1_line_size;
        unsigned int l1_set_index = block_address % l1_num_sets;
        unsigned int l1_tag = block_address / l1_num_sets;

        int hit_l1 = 0;
        int lru_index_l1 = -1;
        int max_lru_l1 = -1;
        int found_invalid = 0;

        for (int i = 0; i < l1_associativity; i++) {
            if (l1_valid[l1_set_index][i] && l1_tags[l1_set_index][i] == l1_tag) {
                l1_hits++;
                hit_l1 = 1;
                l1_lru[l1_set_index][i] = 0; 
                if (operation == 'W' || operation == 'S') {
                    l1_dirty[l1_set_index][i] = 1;
                }
                for (int j = 0; j < l1_associativity; j++) {
                    if (j != i && l1_valid[l1_set_index][j]) {
                        l1_lru[l1_set_index][j]++;
                    }
                }
                break;
            } else {
                if (!l1_valid[l1_set_index][i] && !found_invalid) {
                    lru_index_l1 = i;
                    found_invalid = 1;
                } else if (!found_invalid && l1_lru[l1_set_index][i] > max_lru_l1) {
                    max_lru_l1 = l1_lru[l1_set_index][i];
                    lru_index_l1 = i;
                }
                if (l1_valid[l1_set_index][i]) {
                    l1_lru[l1_set_index][i]++;
                }
            }
        }

        if (!hit_l1) {
            if (l1_valid[l1_set_index][lru_index_l1]) {
                if (l1_dirty[l1_set_index][lru_index_l1]) {
                    l2_accesses++;  

                    unsigned int evicted_block_address = (l1_tags[l1_set_index][lru_index_l1] * l1_num_sets + l1_set_index) * l1_line_size;
                    unsigned int l2_block_address = evicted_block_address / l2_line_size;
                    unsigned int l2_set_index_evicted = l2_block_address % l2_num_sets;
                    unsigned int l2_tag_evicted = l2_block_address / l2_num_sets;

                    int hit_l2_evicted = 0;
                    int lru_index_l2_evicted = -1;
                    int max_lru_l2_evicted = -1;
                    int found_invalid_l2_evicted = 0;

                    for (int i = 0; i < l2_associativity; i++) {
                        if (l2_valid[l2_set_index_evicted][i] && l2_tags[l2_set_index_evicted][i] == l2_tag_evicted) {
                            l2_hits++;
                            hit_l2_evicted = 1;
                            l2_lru[l2_set_index_evicted][i] = 0;  
                            l2_dirty[l2_set_index_evicted][i] = 1;
                            for (int j = 0; j < l2_associativity; j++) {
                                if (j != i && l2_valid[l2_set_index_evicted][j]) {
                                    l2_lru[l2_set_index_evicted][j]++;
                                }
                            }
                            break;
                        } else {
                            if (!l2_valid[l2_set_index_evicted][i] && !found_invalid_l2_evicted) {
                                lru_index_l2_evicted = i;
                                found_invalid_l2_evicted = 1;
                            } else if (!found_invalid_l2_evicted && l2_lru[l2_set_index_evicted][i] > max_lru_l2_evicted) {
                                max_lru_l2_evicted = l2_lru[l2_set_index_evicted][i];
                                lru_index_l2_evicted = i;
                            }
                            if (l2_valid[l2_set_index_evicted][i]) {
                                l2_lru[l2_set_index_evicted][i]++;
                            }
                        }
                    }

                    if (!hit_l2_evicted) {
                        l2_tags[l2_set_index_evicted][lru_index_l2_evicted] = l2_tag_evicted;
                        l2_valid[l2_set_index_evicted][lru_index_l2_evicted] = 1;
                        l2_dirty[l2_set_index_evicted][lru_index_l2_evicted] = 1; 
                        l2_lru[l2_set_index_evicted][lru_index_l2_evicted] = 0;
                        for (int i = 0; i < l2_associativity; i++) {
                            if (i != lru_index_l2_evicted && l2_valid[l2_set_index_evicted][i]) {
                                l2_lru[l2_set_index_evicted][i]++;
                            }
                        }
                    }
                }
            }

            l1_tags[l1_set_index][lru_index_l1] = l1_tag;
            l1_valid[l1_set_index][lru_index_l1] = 1;
            l1_dirty[l1_set_index][lru_index_l1] = (operation == 'W' || operation == 'S') ? 1 : 0;
            l1_lru[l1_set_index][lru_index_l1] = 0;
            for (int i = 0; i < l1_associativity; i++) {
                if (i != lru_index_l1 && l1_valid[l1_set_index][i]) {
                    l1_lru[l1_set_index][i]++;
                }
            }

            l2_accesses++;

            unsigned int block_address_l2 = address / l2_line_size;
            unsigned int l2_set_index = block_address_l2 % l2_num_sets;
            unsigned int l2_tag = block_address_l2 / l2_num_sets;

            int hit_l2 = 0;
            int lru_index_l2 = -1;
            int max_lru_l2 = -1;
            int found_invalid_l2 = 0;

            for (int i = 0; i < l2_associativity; i++) {
                if (l2_valid[l2_set_index][i] && l2_tags[l2_set_index][i] == l2_tag) {
                    l2_hits++;
                    hit_l2 = 1;
                    l2_lru[l2_set_index][i] = 0;  
                    for (int j = 0; j < l2_associativity; j++) {
                        if (j != i && l2_valid[l2_set_index][j]) {
                            l2_lru[l2_set_index][j]++;
                        }
                    }
                    break;
                } else {
                    if (!l2_valid[l2_set_index][i] && !found_invalid_l2) {
                        lru_index_l2 = i;
                        found_invalid_l2 = 1;
                    } else if (!found_invalid_l2 && l2_lru[l2_set_index][i] > max_lru_l2) {
                        max_lru_l2 = l2_lru[l2_set_index][i];
                        lru_index_l2 = i;
                    }
                    if (l2_valid[l2_set_index][i]) {
                        l2_lru[l2_set_index][i]++;
                    }
                }
            }

            if (!hit_l2) {
                if (l2_valid[l2_set_index][lru_index_l2]) {
                    if (l2_dirty[l2_set_index][lru_index_l2]) {
                    }
                }
                l2_tags[l2_set_index][lru_index_l2] = l2_tag;
                l2_valid[l2_set_index][lru_index_l2] = 1;
                l2_dirty[l2_set_index][lru_index_l2] = 0; 
                l2_lru[l2_set_index][lru_index_l2] = 0;
                for (int i = 0; i < l2_associativity; i++) {
                    if (i != lru_index_l2 && l2_valid[l2_set_index][i]) {
                        l2_lru[l2_set_index][i]++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < l1_num_sets; i++) {
        free(l1_tags[i]);
        free(l1_lru[i]);
        free(l1_valid[i]);
        free(l1_dirty[i]);
    }
    free(l1_tags);
    free(l1_lru);
    free(l1_valid);
    free(l1_dirty);

    for (int i = 0; i < l2_num_sets; i++) {
        free(l2_tags[i]);
        free(l2_lru[i]);
        free(l2_valid[i]);
        free(l2_dirty[i]);
    }
    free(l2_tags);
    free(l2_lru);
    free(l2_valid);
    free(l2_dirty);

    fprintf(output_file, "%d,%d;%d,%d\n", l1_hits, l1_accesses, l2_hits, l2_accesses);
}

void calculateL2Utilization(FILE* input_file, FILE* output_file) {
    int l2_cache_size = 64 * 1024;    
    int l2_line_size = 64;           
    int total_l2_lines = l2_cache_size / l2_line_size; 

    int l1_cache_size = 4 * 1024;    
    int l1_line_size = 32;           
    int l1_associativity = 4;        
    int l1_num_sets = l1_cache_size / (l1_line_size * l1_associativity);

    int associativities[] = {8, total_l2_lines, 1 }; 

    for (int config = 0; config < 3; config++) {
        int l2_associativity = associativities[config];
        int l2_num_sets = total_l2_lines / l2_associativity;

        unsigned int **l1_tags = malloc(l1_num_sets * sizeof(unsigned int *));
        int **l1_lru = malloc(l1_num_sets * sizeof(int *));
        int **l1_valid = malloc(l1_num_sets * sizeof(int *));
        int **l1_dirty = malloc(l1_num_sets * sizeof(int *));  // Dirty bits
        for (int i = 0; i < l1_num_sets; i++) {
            l1_tags[i] = calloc(l1_associativity, sizeof(unsigned int));
            l1_lru[i] = calloc(l1_associativity, sizeof(int));
            l1_valid[i] = calloc(l1_associativity, sizeof(int));
            l1_dirty[i] = calloc(l1_associativity, sizeof(int));
        }

        unsigned int **l2_tags = malloc(l2_num_sets * sizeof(unsigned int *));
        int **l2_lru = malloc(l2_num_sets * sizeof(int *));
        int **l2_valid = malloc(l2_num_sets * sizeof(int *));
        for (int i = 0; i < l2_num_sets; i++) {
            l2_tags[i] = calloc(l2_associativity, sizeof(unsigned int));
            l2_lru[i] = calloc(l2_associativity, sizeof(int));
            l2_valid[i] = calloc(l2_associativity, sizeof(int));
        }

        fseek(input_file, 0, SEEK_SET);

        int l1_accesses = 0, l1_hits = 0;
        int l2_accesses = 0, l2_hits = 0;

        char operation;
        unsigned int address;
        while (fscanf(input_file, " %c %x", &operation, &address) == 2) {
            if (operation >= 'a' && operation <= 'z') {
                operation -= 'a' - 'A';
            }

            l1_accesses++;  

            unsigned int block_address = address / l1_line_size;
            unsigned int l1_set_index = block_address % l1_num_sets;
            unsigned int l1_tag = block_address / l1_num_sets;

            int hit_l1 = 0;
            int lru_index_l1 = -1;
            int max_lru_l1 = -1;
            int found_invalid = 0;

            for (int i = 0; i < l1_associativity; i++) {
                if (l1_valid[l1_set_index][i] && l1_tags[l1_set_index][i] == l1_tag) {
                    l1_hits++;
                    hit_l1 = 1;
                    l1_lru[l1_set_index][i] = 0;  
                    if (operation == 'W' || operation == 'S') {
                        l1_dirty[l1_set_index][i] = 1;
                    }
                    for (int j = 0; j < l1_associativity; j++) {
                        if (j != i && l1_valid[l1_set_index][j]) {
                            l1_lru[l1_set_index][j]++;
                        }
                    }
                    break;
                } else {
                    if (!l1_valid[l1_set_index][i] && !found_invalid) {
                        lru_index_l1 = i;
                        found_invalid = 1;
                    } else if (!found_invalid && l1_lru[l1_set_index][i] > max_lru_l1) {
                        max_lru_l1 = l1_lru[l1_set_index][i];
                        lru_index_l1 = i;
                    }
                }
            }

            if (!hit_l1) {
                if (l1_valid[l1_set_index][lru_index_l1]) {
                    if (l1_dirty[l1_set_index][lru_index_l1]) {
                        l2_accesses++;  

                        unsigned int evicted_block_address = (l1_tags[l1_set_index][lru_index_l1] * l1_num_sets + l1_set_index) * l1_line_size;
                        unsigned int l2_block_address = evicted_block_address / l2_line_size;
                        unsigned int l2_set_index_evicted = (l2_num_sets > 1) ? l2_block_address % l2_num_sets : 0;
                        unsigned int l2_tag_evicted = l2_block_address / l2_num_sets;

                        int hit_l2_evicted = 0;
                        int lru_index_l2_evicted = -1;
                        int max_lru_l2_evicted = -1;
                        int found_invalid_l2_evicted = 0;

                        for (int i = 0; i < l2_associativity; i++) {
                            if (l2_valid[l2_set_index_evicted][i] && l2_tags[l2_set_index_evicted][i] == l2_tag_evicted) {
                                l2_hits++;
                                hit_l2_evicted = 1;
                                l2_lru[l2_set_index_evicted][i] = 0; 
                                for (int j = 0; j < l2_associativity; j++) {
                                    if (j != i && l2_valid[l2_set_index_evicted][j]) {
                                        l2_lru[l2_set_index_evicted][j]++;
                                    }
                                }
                                break;
                            } else {
                                if (!l2_valid[l2_set_index_evicted][i] && !found_invalid_l2_evicted) {
                                    lru_index_l2_evicted = i;
                                    found_invalid_l2_evicted = 1;
                                } else if (!found_invalid_l2_evicted && l2_lru[l2_set_index_evicted][i] > max_lru_l2_evicted) {
                                    max_lru_l2_evicted = l2_lru[l2_set_index_evicted][i];
                                    lru_index_l2_evicted = i;
                                }
                            }
                        }

                        if (!hit_l2_evicted) {
                            l2_tags[l2_set_index_evicted][lru_index_l2_evicted] = l2_tag_evicted;
                            l2_valid[l2_set_index_evicted][lru_index_l2_evicted] = 1;
                            l2_lru[l2_set_index_evicted][lru_index_l2_evicted] = 0;
                            for (int i = 0; i < l2_associativity; i++) {
                                if (i != lru_index_l2_evicted && l2_valid[l2_set_index_evicted][i]) {
                                    l2_lru[l2_set_index_evicted][i]++;
                                }
                            }
                        }
                    }
                }

                l1_tags[l1_set_index][lru_index_l1] = l1_tag;
                l1_valid[l1_set_index][lru_index_l1] = 1;
                l1_dirty[l1_set_index][lru_index_l1] = (operation == 'W' || operation == 'S') ? 1 : 0;
                l1_lru[l1_set_index][lru_index_l1] = 0;
                for (int i = 0; i < l1_associativity; i++) {
                    if (i != lru_index_l1 && l1_valid[l1_set_index][i]) {
                        l1_lru[l1_set_index][i]++;
                    }
                }

                l2_accesses++;

                unsigned int block_address_l2 = address / l2_line_size;
                unsigned int l2_set_index = (l2_num_sets > 1) ? block_address_l2 % l2_num_sets : 0;
                unsigned int l2_tag = block_address_l2 / l2_num_sets;

                int hit_l2 = 0;
                int lru_index_l2 = -1;
                int max_lru_l2 = -1;
                int found_invalid_l2 = 0;

                for (int i = 0; i < l2_associativity; i++) {
                    if (l2_valid[l2_set_index][i] && l2_tags[l2_set_index][i] == l2_tag) {
                        l2_hits++;
                        hit_l2 = 1;
                        l2_lru[l2_set_index][i] = 0; 
                        for (int j = 0; j < l2_associativity; j++) {
                            if (j != i && l2_valid[l2_set_index][j]) {
                                l2_lru[l2_set_index][j]++;
                            }
                        }
                        break;
                    } else {
                        if (!l2_valid[l2_set_index][i] && !found_invalid_l2) {
                            lru_index_l2 = i;
                            found_invalid_l2 = 1;
                        } else if (!found_invalid_l2 && l2_lru[l2_set_index][i] > max_lru_l2) {
                            max_lru_l2 = l2_lru[l2_set_index][i];
                            lru_index_l2 = i;
                        }
                    }
                }

                if (!hit_l2) {
                    l2_tags[l2_set_index][lru_index_l2] = l2_tag;
                    l2_valid[l2_set_index][lru_index_l2] = 1;
                    l2_lru[l2_set_index][lru_index_l2] = 0;
                    for (int i = 0; i < l2_associativity; i++) {
                        if (i != lru_index_l2 && l2_valid[l2_set_index][i]) {
                            l2_lru[l2_set_index][i]++;
                        }
                    }
                }
            }
        }

        int occupied_cache_lines = 0;
        for (int i = 0; i < l2_num_sets; i++) {
            for (int j = 0; j < l2_associativity; j++) {
                if (l2_valid[i][j]) {
                    occupied_cache_lines++;
                }
            }
        }

        double utilization = (double)occupied_cache_lines / total_l2_lines;

        fprintf(output_file, "%d,%d;%d,%d; %.6f\n", l1_hits, l1_accesses, l2_hits, l2_accesses, utilization);

        for (int i = 0; i < l1_num_sets; i++) {
            free(l1_tags[i]);
            free(l1_lru[i]);
            free(l1_valid[i]);
            free(l1_dirty[i]);
        }
        free(l1_tags);
        free(l1_lru);
        free(l1_valid);
        free(l1_dirty);

        for (int i = 0; i < l2_num_sets; i++) {
            free(l2_tags[i]);
            free(l2_lru[i]);
            free(l2_valid[i]);
        }
        free(l2_tags);
        free(l2_lru);
        free(l2_valid);
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Incorrect input type \n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    if(input_file == NULL){
        printf("Cannot open file \n");
        return 1;
    }
    
    directMapped(input_file, output_file);
    rewind(input_file);

    setAssociative(input_file, output_file);
    rewind(input_file);

    fullyAssociative(input_file, output_file);
    rewind(input_file);

    twoLevelCache(input_file, output_file);
    rewind(input_file);

    twoLevelCacheWriteBack(input_file, output_file);
    rewind(input_file);

    calculateL2Utilization(input_file, output_file);


    fclose(input_file);
    fclose(output_file);

    return 0;
}