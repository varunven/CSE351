/*
CSE 351 Lab 4, Part 1 - Mystery Caches

Varun Venkatesh
varunven
*/

#include <stdlib.h>
#include <stdio.h>

#include "support/mystery-cache.h"

/*
 * NOTES: 
 * 1. When using access_cache() you do not need to provide a "real" memory
 * addresses. You can use any convenient integer value as a memory address,
 * you should not be able to cause a segmentation fault by providing a memory
 * address out of your programs address space as the argument to access_cache.
 * 
 * 2. Do NOT change the provided main function, especially the print statement.
 * If you do so, the autograder may fail to grade your code even if it produces
 * the correct result.
 */

/*
   Returns the size (in B) of each block in the cache.
*/
int get_block_size(void)
{
    int curr = 0;
    int size = 0;
    flush_cache();
    access_cache(0);
    while (access_cache(curr))
    {
        size++;
        curr++;
    }
    return size;
}

/*
   Returns the size (in B) of the cache.
*/
int get_cache_size(int block_size)
{
    int curr = 0;
    int cache_check = block_size;
    flush_cache();
    access_cache(0);
    while (access_cache(0))
    {
        curr = block_size;
        while (curr <= cache_check)
        {
            curr += block_size;
            access_cache(curr);
        }
        cache_check += block_size;
    }
    return curr;
}

/*
   Returns the associativity of the cache.
*/
int get_cache_assoc(int cache_size)
{
    int curr = 0;
    int cache_check = 1;
    int associativity = 0;
    flush_cache();
    access_cache(0);
    while (access_cache(0))
    {
        curr = cache_size;
        associativity = 0;
        while (curr <= cache_check)
        {
            curr += cache_size;
            associativity++;
            access_cache(curr);
        }
        cache_check += cache_size;
    }
    return associativity;
}

int main(void)
{
    int size;
    int assoc;
    int block_size;

    cache_init(0, 0);

    block_size = get_block_size();
    size = get_cache_size(block_size);
    assoc = get_cache_assoc(size);

    printf("Cache block size: %d bytes\n", block_size);
    printf("Cache size: %d bytes\n", size);
    printf("Cache associativity: %d\n", assoc);

    return EXIT_SUCCESS;
}
