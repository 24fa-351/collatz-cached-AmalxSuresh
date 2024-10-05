#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

// #define LARGEST_HASH 100003
 #define MAX_CACHE_SIZE 100000000 // Adjust as needed

typedef struct {
    uint64_t key;
    uint64_t value;
    int used; // 0 if not used, 1 if used
} cache_entry;

typedef struct {
    cache_entry entries[MAX_CACHE_SIZE];
    uint64_t current_size;
    uint64_t cache_capacity;
} cache_main;

cache_main* create_cache(uint64_t cache_capacity);
void evict_value(cache_main* cache, char* policy);
void add_entry(cache_main* cache, uint64_t key, uint64_t value, char* policy);
int get_cache_value(cache_main* cache, uint64_t key, char*);
void free_cache(cache_main* cache);
int collatz_cached(cache_main* cache, long long int random_number, int* cache_hits, int* cache_misses, char *policy);
