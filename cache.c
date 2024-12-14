#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "cache.h"

#define MAX_CACHE_SIZE 100000000
#define CACHE_MISS -1

// Create cache
cache_main* create_cache(uint64_t cache_capacity){
    cache_main* cache = (cache_main*)malloc(sizeof(cache_main));
    if (cache == NULL) {
        fprintf(stderr, "Memory allocation failed for cache.\n");
        return NULL;
    }
    for (int ix = 0; ix < cache_capacity; ix++){
        cache->entries[ix].used = 0;
    }
    cache -> current_size = 0;
    cache -> cache_capacity = cache_capacity;

    return cache;
}


//evict value based on policy
void evict_value(cache_main* cache, char* policy){
    if (cache -> current_size == 0){
        return; 
    }
    if (strcmp(policy, "rr") == 0 || strcmp(policy, "RR") == 0){
        int random_index = rand() % cache->current_size;
        memset(&cache -> entries[random_index], 0, sizeof(cache_entry));    

        if (random_index < cache -> current_size - 1){
            memmove(&cache->entries[random_index], &cache -> entries[random_index + 1], 
                    (cache -> current_size - random_index - 1) * sizeof(cache_entry));
        }
    }
    if (strcmp(policy, "lru") == 0 || strcmp(policy, "LRU") == 0){
        memmove(&cache -> entries[0], &cache->entries[1], 
                (cache -> current_size - 1) * sizeof(cache_entry));
        memset(&cache -> entries[cache -> current_size - 1], 0, sizeof(cache_entry));
    }
    cache -> current_size--; // Decrement size after eviction
}


void add_entry(cache_main* cache, uint64_t key, uint64_t value, char* policy) {
    if (cache -> current_size == cache -> cache_capacity){
        evict_value(cache, policy);
    }
    cache -> entries[cache->current_size].key = key;
    cache -> entries[cache->current_size].value = value;
    cache -> entries[cache->current_size].used = 1;
    cache -> current_size++;
}

int get_cache_value(cache_main* cache, uint64_t key, char *policy){
    if (strcmp(policy, "rr") == 0 || strcmp(policy, "RR") == 0) {
        for (int ix = 0; ix < cache -> current_size; ix++) { // Change to current_size
            if (cache -> entries[ix].used && cache -> entries[ix].key == key) {
                return cache -> entries[ix].value;
            }
        }
    }
    if (strcmp(policy, "lru") == 0 || strcmp(policy, "LRU") == 0) {
        for (int ix = 0; ix < cache -> current_size; ix++) { // Change to current_size
            if (cache -> entries[ix].used && cache -> entries[ix].key == key) {
                cache_entry temp = cache -> entries[ix];
                memmove(&cache -> entries[ix], &cache -> entries[ix + 1], (cache -> current_size - ix - 1) * sizeof(cache_entry));
                cache -> entries[cache -> current_size - 1] = temp;

                return temp.value;
            }
        }
    }
    return CACHE_MISS;
}



void free_cache(cache_main* cache) {
    free(cache);
}

// Collatz core function
long long int collatz_core(long long int random_number){
    long long int collatz_steps = 0;
    while (random_number > 1){
        if (random_number % 2 == 0){
            random_number = random_number / 2;
        } else{
            random_number = random_number * 3 + 1;
        }
        collatz_steps++;
    }
    return collatz_steps;
}

// main collatz function
int collatz_cached(cache_main* cache, long long int random_number, int* cache_hits, int* cache_misses, char *policy){
    int steps = 0;
    int cached_steps = get_cache_value(cache, random_number, policy);
    if (cached_steps != -1){
        (*cache_hits)++;
        return cached_steps;
    }

    (*cache_misses)++;
    steps = collatz_core(random_number);
    add_entry(cache, random_number, steps, policy); 

    return steps;
}

