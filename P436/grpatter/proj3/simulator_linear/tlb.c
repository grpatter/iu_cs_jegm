/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"

tlb_item_t *tlb = NULL;

static int num_tlb_entries = 0;

/*********************************************************
 * TLB
 *********************************************************/
int allocate_tlb(int tlb_entries) {
    num_tlb_entries = tlb_entries;

    tlb = (tlb_item_t*)malloc(sizeof(tlb_item_t) * num_tlb_entries);
    if( NULL == tlb ) {
        fprintf(stderr, "Error: Unable to allocate the TLB data structure\n");
        return -1;
    }

    flush_tlb();

    return 0;
}

int free_tlb(void) {
    free(tlb);
    tlb = NULL;
    return 0;
}

void flush_tlb(void) {
    int i;

    /*
     * Simulate time needed to flush TLB
     * - Assume it occurs in a single access
     */
    nanosleep(&ACCESS_TIME_TLB, NULL);

    for( i = 0; i < num_tlb_entries; ++i ) {
        tlb[i].page  = 0;
        tlb[i].frame = 0;
        tlb[i].owner = 0;
        tlb[i].valid = false;
    }
}

void add_to_tlb(page_t page, frame_t frame, pid_t pid) {
    static int next_victim = 0;

    /*
     * Simulate time needed to access TLB
     */
    nanosleep(&ACCESS_TIME_TLB, NULL);

    tlb[next_victim].page  = page;
    tlb[next_victim].frame = frame;
    tlb[next_victim].owner = pid;
    tlb[next_victim].valid = true;

    next_victim = (next_victim+1)%num_tlb_entries;
}

void remove_from_tlb(page_t page, pid_t pid) {
    int i;

    /*
     * Simulate time needed to access TLB
     * - Assume single access time
     */
    nanosleep(&ACCESS_TIME_TLB, NULL);

    for( i = 0; i < num_tlb_entries; ++i ) {
        if( tlb[i].valid ) {
            if( tlb[i].owner == pid && tlb[i].page == page ) {
                tlb[i].valid = false;
                return;
            }
        }
    }

    /*
     * If we got here then the page was not in TLB.
     * It was probably flushed out by another memory access.
     */
}

int check_tlb(pid_t pid, char mode, addr_t address, frame_t *frame) {
    int i;

    /*
     * Simulate time needed to access TLB
     */
    nanosleep(&ACCESS_TIME_TLB, NULL);

    /*
     * Check the TLB to see if the page translation is available
     */
    for( i = 0; i < num_tlb_entries; ++i ) {
        if( tlb[i].valid && tlb[i].owner == pid ) {
            if( tlb[i].page == GET_PAGE(address) ) {
                /* TLB Hit
                 * - Update the Cache
                 * - Updating a write-through cache, updates RAM for us
                 */
                add_to_cache(tlb[i].owner, tlb[i].page, tlb[i].frame, mode);

                *frame = tlb[i].frame;
                return 1;
            }
        }
    }

    return 0;
}
