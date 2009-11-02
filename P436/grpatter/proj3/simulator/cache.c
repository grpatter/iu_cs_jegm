/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"

/*
 * Cache array
 */
cache_item_t *cache = NULL;
static int num_cache_entries = 0;

/*
 * Debugging routines to display the cache
 */
static void display_cache_entry(int idx, bool header);
static void display_cache(void);

/*
 * Correctness check to make sure that a page in cache is also in RAM
 */
static void check_cache_valid(int idx);

/*********************************************************
 * Cache
 *********************************************************/
int allocate_cache(int cache_entries) {

    num_cache_entries = cache_entries;

    cache = (cache_item_t*)malloc(sizeof(cache_item_t) * num_cache_entries);
    if( NULL == cache ) {
        fprintf(stderr, "Error: Unable to allocate the cache data structure\n");
        return -1;
    }

    flush_cache();

    return 0;
}

int free_cache(void) {
    free(cache);
    cache = NULL;
    return 0;
}

void flush_cache(void) {
    int i;

    /*
     * Simulate time needed to access Cache
     * - Assume this occurs in a single access
     */
    nanosleep(&ACCESS_TIME_CACHE, NULL);

    for( i = 0; i < num_cache_entries; ++i ) {
        cache[i].page    = 0;
        cache[i].frame   = 0;
        cache[i].owner   = 0;
        cache[i].dirty   = false;
        cache[i].valid   = false;
        cache[i].ref     = false;
    }
}

void add_to_cache(pid_t pid, page_t page, frame_t frame, char mode) {
    static int next_victim = 0;

    /*
     * Simulate time needed to access Cache
     */
    nanosleep(&ACCESS_TIME_CACHE, NULL);

    cache[next_victim].page  = page;
    cache[next_victim].frame = frame;
    cache[next_victim].owner = pid;
    cache[next_victim].valid = true;

    if( mode == 'W' ) {
        cache[next_victim].dirty  = true;
        physical_mem[frame].dirty = true;
    } else {
        cache[next_victim].dirty = physical_mem[frame].dirty;
    }

    cache[next_victim].ref  = true;
    physical_mem[frame].ref = true;

    /*
     * Assume time to write-through to main memory is handled asynchronously for
     * us by the hardware, so we do not need to account for it.
     */

    next_victim = (next_victim+1)%num_cache_entries;
}

void remove_from_cache(pid_t pid, page_t page) {
    int i;

    /*
     * Simulate time needed to access Cache
     * - Assume single access time
     */
    nanosleep(&ACCESS_TIME_CACHE, NULL);

    for( i = 0; i < num_cache_entries; ++i ) {
        if( cache[i].valid ) {
            if( cache[i].owner == pid && cache[i].page == page ) {
                /* Clear entry */
                cache[i].valid   = false;
                return;
            }
        }
    }

    /*
     * If we got here then the page was not in cache.
     * It was probably flushed out by another memory access.
     */
}

int check_cache(pid_t pid, char mode, addr_t address, frame_t *frame) {
    int i;

    /*
     * Simulate time needed to access Cache
     */
    nanosleep(&ACCESS_TIME_CACHE, NULL);

    /*
     * Check the cache to see if the page is resident
     */
    for( i = 0; i < num_cache_entries; ++i ) {
        if( cache[i].valid ) {
            if( cache[i].owner == pid && cache[i].page == GET_PAGE(address) ) {
                /* Write through to RAM */
                if( mode == 'W' ) {
                    cache[i].dirty = true;
                    physical_mem[cache[i].frame].dirty = true;
                }
                cache[i].ref = true;
                physical_mem[cache[i].frame].ref = true;

                /* Sanity Check */
                check_cache_valid(i);

                *frame = cache[i].frame;
                return 1;
            }
        }
    }

    return 0;
}

/******************************/
static void display_cache(void) {
    int i;

    printf("Cache Entry: Page , Frame, D, R, V, PID\n");
    printf("---------------------------------------\n");
    for( i = 0; i < num_cache_entries; ++i ) {
        if( cache[i].valid ) {
            display_cache_entry(i, false);
        }
    }
}

static void display_cache_entry(int idx, bool header) {

    if( header ) {
        printf("Cache Entry: Page , Frame, D, R, V, PID\n");
        printf("---------------------------------------\n");
    }
    printf("CACHE %4d : %5d, %5d, %c, %c, %c, %5d\n",
           idx,
           cache[idx].page,
           cache[idx].frame,
           (cache[idx].dirty ? 'T' : 'F'),
           (cache[idx].ref ? 'T' : 'F'),
           (cache[idx].valid ? 'T' : 'F'),
           cache[idx].owner);
}

static void check_cache_valid(int idx) {
    /*
     * Make sure it is in the page table!
     */
    if( 0 != assert_page_in_ram(cache[idx].page) ) {
        fprintf(stderr, "Error: We had a Cache Hit without a page in RAM!\n");
        exit(-1);
    }
}
