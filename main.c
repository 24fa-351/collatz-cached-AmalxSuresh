#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include "cache.h"

long long int random_num(long long int largest_value, long long int smallest_value);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    double hit_percent = 0;
    int cache_hits = 0, cache_misses = 0;
    long long int num_values, smallest_value, largest_value, random_number, cache_size;
    
    sscanf(argv[1], "%lld", &num_values);
    sscanf(argv[2], "%lld", &smallest_value);
    sscanf(argv[3], "%lld", &largest_value);
    sscanf(argv[4], "%lld", &cache_size);
    char *policy = argv[5];    
    
    long long int* steps_array = (long long int*)malloc(num_values * sizeof(long long int));

    printf("min = %lld, max = %lld, number of entries = %lld, cache_size = %lld \n", smallest_value, largest_value, num_values, cache_size);

    cache_main* cache = create_cache(cache_size);

    for (int i = 0; i < num_values; i++) {
        random_number = random_num(largest_value, smallest_value);
        steps_array[i] = collatz_cached(cache, random_number, &cache_hits, &cache_misses, policy);
    }
    hit_percent = (double) cache_hits / (cache_hits + cache_misses);

    printf("Cache hit percentage is : %.2f\n", hit_percent);
    
    free_cache(cache);
}

long long int random_num(long long int largest_value, long long int smallest_value) {
    long long int random_number = rand() % (largest_value - smallest_value + 1) + smallest_value;  
    return random_number;
}

