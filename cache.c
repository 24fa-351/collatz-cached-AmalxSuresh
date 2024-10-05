#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "cache.h"

#define MAX_CACHE_SIZE 100000000

// Create cache
cache_main* create_cache(uint64_t cache_capacity){
    cache_main* cache = (cache_main*)malloc(sizeof(cache));
    for (int i = 0; i < cache_capacity; i++){
        cache->entries[i].used = 0; 
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
    //remove random entry from cache and shift all elements left
    if(strcmp(policy, "rr") == 0 || strcmp(policy, "RR") == 0){
        int random_index = rand() % cache->current_size;
        memset(&cache -> entries[random_index], 0, sizeof(cache_entry));    

        if(random_index < cache -> current_size -1){
            memmove(&cache->entries[random_index], &cache -> entries[random_index + 1], (cache -> current_size - random_index - 1) * sizeof(cache_entry));
        }
    }
    //remove least recently used element and shift all elements left
    if(strcmp(policy, "lru") == 0 || strcmp(policy, "LRU") == 0){
        memmove(&cache -> entries[0], &cache->entries[1], (cache -> current_size - 1) * sizeof(cache_entry));
        memset(&cache -> entries[cache -> current_size - 1], 0, sizeof(cache_entry));
    }

    cache -> current_size--;
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
    //for rr only access element and move on
    if(strcmp(policy, "rr") == 0 || strcmp(policy, "RR") == 0){

        for( int ix = 0; ix < cache -> cache_capacity; ix++){

            if(cache -> entries[ix].used && cache -> entries[ix].key == key){
                return cache -> entries[ix].value;
            }
        }
    }
    //for lru access element and move it to the front of cache
    if(strcmp(policy, "lru") == 0 || strcmp(policy, "LRU") == 0){

        for (int i = 0; i < cache -> cache_capacity; i++){

            if (cache -> entries[i].used && cache -> entries[i].key == key){
                cache_entry temp = cache -> entries[i];
                memmove(&cache -> entries[i], &cache -> entries[i + 1], (cache -> current_size - i - 1) * sizeof(cache_entry));
                cache -> entries[cache -> current_size - 1] = temp;
            
                return temp.value;
            }
        }
    }
    
    return -1;
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
        printf("steps %d - ", cached_steps);
        return cached_steps;
    }

    (*cache_misses)++;
    steps = collatz_core(random_number);
    add_entry(cache, random_number, steps, policy); 

    return steps;
}

