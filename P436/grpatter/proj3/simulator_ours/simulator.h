/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 * This header file contains data structures and types necessary for the
 * simulator. While completing the assignment you may need to slightly alter
 * these data structures to complete the assignment.
 * 
 */
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "support.h"

/*
 * PIDs range from 0 to 99
 */
#define MAX_PID_LEN   2

/*
 * The maximum line in the input file
 */
#define MAX_LINE    128

/*
 * Page Size in the system is 4K
 */
#define PAGE_SIZE       4096
#define FRAME_SIZE_BITS 12
#define PAGE_SIZE_BITS  12
#define PAGE_REF        (32 - PAGE_SIZE_BITS) 

/*
 * Frame: Upper 20 bits of the physical address
 */
#define GET_FRAME(hex)   (hex>>FRAME_SIZE_BITS)

/*
 * Page: Upper 20 bits of the virtual address
 */
#define GET_PAGE(hex)   (hex>>PAGE_SIZE_BITS)

/*
 * Offset: Lower 12 bits of the virtual address
 */
#define GET_OFFSET(hex) (hex&0xfff)

/*
 * Delays in the system. Meant to be passed to nanosleep()
 * Cache:  2 ns
 * TLB:   10 ns
 * RAM:   70 ns
 * DISK:  10 ms = 10,000,000 ns
 */
const struct timespec ACCESS_TIME_CACHE = {0, 2};
const struct timespec ACCESS_TIME_TLB   = {0, 10};
const struct timespec ACCESS_TIME_RAM   = {0, 70};
const struct timespec ACCESS_TIME_DISK  = {0, 10000000};


/*
 * Cache:
 *    64 = 256 KB
 *   512 = 2 MB
 * Cache Size: Page Size * Cache Entries
 */
#define DEFAULT_CACHE_ENTRIES 64

/*
 * TLB Size:
 *  64 - 1024 entries
 */
#define DEFAULT_TLB_ENTRIES 64

/*
 * Default 2 GB of Virtual Address space
 *  1-Level Page Table: (2*1024*1024*1024)/4096 = 524288 Entries
 */
#define DEFAULT_NUM_PAGES 524288

/*
 * Default 64 MB of Physical Memory
 * (64*1024*1024)/4096 = 16384 frames
 */
#define DEFAULT_NUM_FRAMES 16384

/*
 * Default Random Page Evition Algo
 */
#define DEFAULT_PAGE_ALGO "RANDOM"

/*
 * Generic address type
 */
typedef int32_t addr_t;
/*
 * Generic page type
 */
typedef int32_t page_t;
/*
 * Generic frame type
 */
typedef page_t  frame_t;

/*
 * Cache Item data structure
 */
typedef struct {
    page_t   page;
    frame_t  frame;
    pid_t    owner;
    bool     dirty;
    bool     valid;
    bool     ref;
} cache_item_t;

/*
 * TLB Item data structure
 */
typedef struct {
    page_t   page;
    frame_t  frame;
    pid_t    owner;
    bool     valid;
} tlb_item_t;

/*
 * Page Table Leaf Item
 */
typedef struct {
    pid_t    pid;
    page_t   page;
    frame_t  frame;
    bool     valid;
} page_table_leaf_t;

/*
 * RAM Info data structure
 */
typedef struct {
	int n_frames;
	char *algorithm;
}ram_info_t;

/*
 * Page Table
 */
typedef struct {
    /* Array of page <-> frame leafs */
    page_table_leaf_t *pages;
} page_table_t;

/*
 * System wide process page table container
 */
typedef struct {
    /* Process that owns this page table */
    pid_t    pid;
    /* Pointer to the page table in memory */
    page_table_t *page_table;
} proc_page_table_ref_t;

/*
 * Structure representing a frame in Physical Memory
 */
typedef struct {
    frame_t  frame;
    bool dirty;
    bool ref;
    page_table_leaf_t *page_ref;
} frame_ref_t;

/*
 * Structure for keeping track of stats
 */
typedef struct {
    int cache_miss;
    int cache_hit;
    double cache_hit_ratio;

    int tlb_miss;
    int tlb_hit;
    double tlb_hit_ratio;

    int pt_hit;
    int page_faults;
    double page_fault_rate;

    int swap_out;
    int swap_in;
    double swap_out_ratio;

    int num_procs;
    int num_context_switch;

    int num_errors;

    struct timeval start;
    struct timeval end;

    int cache_size;
    int tlb_size;
    int num_pages;
    int num_frames;
} stat_entry_t;

/*
 * Stats collector
 */
extern stat_entry_t stats;

/*
 * Cache array
 */
extern cache_item_t *cache;

/*
 * TLB array
 */
extern tlb_item_t *tlb;

/*
 * System wide process page table array
 */
extern proc_page_table_ref_t *sys_proc_table;

/*
 * Physical memory array
 */
extern frame_ref_t *physical_mem;

/*
 * String representing the current virtual memory reference
 */
extern char *current_ref;

/**********************
 * Stats Functions
 ************************/
/*
 * Pretty print function
 */
void pretty_print(int32_t value, int32_t mult);

/*
 * Clear the stats structure
 */
void clear_stats(void);

/*
 * Display the stats
 */
void display_stats(void);

/**********************
 * Cache Functions
 ************************/

/*
 * Allocate and initialize cache
 */
int allocate_cache(int cache_entries);

/*
 * Free cache
 */
int free_cache(void);

/*
 * Clear all of the entries in the cache
 */
void  flush_cache(void);

/*
 * Add a page <-> frame translation to cache
 */
void  add_to_cache(pid_t pid, page_t page, frame_t frame, char mode);

/*
 * Invalidate a cache reference
 */
void  remove_from_cache(pid_t pid, page_t page);

/*
 * Check the cache for the necessary page
 */
int   check_cache(pid_t pid, char mode, addr_t address, frame_t *frame);

/**********************
 * TLB Functions
 ************************/
/*
 * Allocate and initialize TLB
 */
int allocate_tlb(int tlb_entries);

/*
 * Free TLB
 */
int free_tlb(void);

/*
 * Clear all of the TLB entries
 */
void flush_tlb(void);

/*
 * Add a page <-> frame translation
 */
void add_to_tlb(page_t page, frame_t frame, pid_t pid);

/*
 * Remove a translation from the TLB
 */
void remove_from_tlb(page_t page, pid_t pid);

/*
 * Check the TLB for resolution of the VM address
 */
int  check_tlb(pid_t pid, char mode, addr_t address, frame_t *frame);

/**********************
 * PAGE_TABLE
 ************************/
/*
 * Allocate and initialize page table
 */
int allocate_page_table(int cache_entries);

/*
 * Free the page table
 */
int free_page_table(void);

/*
 * Check the page table for a translation
 * This should always succeed since if the page is not in memory then we will
 * swap it in.
 */
int  check_page_table(pid_t pid, char mode, addr_t address, frame_t *frame);

/*
 * Check the validity of the address
 */
int check_address(int addr);

/**********************
 * Frame Management
 ************************/
/*
 * Allocate physical memory
 */
//int allocate_ram(int cache_entries);
int allocate_ram(ram_info_t ram_info);

/*
 * Free phsyical memory
 */
int free_ram(void);

/*
 * Simulate a page fault
 */
int page_fault(page_table_leaf_t *page_entry);

/*
 * Debugging option to display phsysical memory
 */
void display_ram(void);

/*
 * Assertion that a page is in RAM
 *  0: If in RAM
 * -1: otherwise
 */
int assert_page_in_ram(page_t page);

#endif
