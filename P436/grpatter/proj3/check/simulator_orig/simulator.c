/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"

/*
 * String representing the current virtual memory reference
 */
char *current_ref = NULL;

/*
 * Parse Command Line Arguments
 */
static int parse_args(int argc, char **argv,
                      char **access_file,
                      int *n_pages, int *n_frames,
                      int *n_cache, int *n_tlb);

/*
 * Extract argument set from the input file
 */
static int extract_args(char *command, pid_t *pid, char *mode, addr_t *address);

/*
 * Central function to translate a virtual address to a physical address
 */
static int access_page(pid_t pid, char mode, addr_t address, frame_t *frame);

/*
 * Last PID marker to determine if we need to context switch
 */
static pid_t last_pid = -1;

/********************** ************************/
int main(int argc,char *argv[]) {
    int n_pages  = 0;
    int n_frames = 0;
    int n_cache  = 0;
    int n_tlb    = 0;
    char *access_file = NULL;
    FILE *access_fd = NULL;
    addr_t virtual_addr = 0;
    addr_t physical_addr;
    pid_t pid = 0;
    char mode;

    /*
     * Parse arguments
     */
    if( 0 != parse_args(argc, argv, &access_file, &n_pages, &n_frames, &n_cache, &n_tlb) ) {
        return -1;
    }

    /*
     * Setup data structures
     */
    srand(time(NULL));
    current_ref = (char *)malloc(sizeof(char) * MAX_LINE);
    clear_stats();

    stats.cache_size = n_cache;
    stats.tlb_size   = n_tlb;
    stats.num_pages  = n_pages;
    stats.num_frames = n_frames;

    allocate_cache(n_cache);
    allocate_tlb(n_tlb);
    allocate_page_table(n_pages);
    allocate_ram(n_frames);

    /*
     * Open the file that we are going to read
     */
    if( NULL == (access_fd = fopen(access_file, "r") ) ) {
        fprintf(stderr, "Error: Unable to open the access file <%s>\n", access_file);
        return -1;
    }

    /*
     * Read page requests from the file
     */
    gettimeofday(&stats.start, NULL);

    while(0 == feof(access_fd) ) {
        /* Read one line */
        current_ref[0] = '\0';
        if( NULL == fgets(current_ref, MAX_LINE, access_fd) ) {
            break;
        }

        /* Strip off the newline */
        if( '\n' == current_ref[strlen(current_ref)-1] ) {
            current_ref[strlen(current_ref)-1] = '\0';
        }

        extract_args(current_ref, &pid, &mode, &virtual_addr);

        /*
         * Memory management operations to access the page
         */
        printf("-----------------------------------------------------------\n");
        printf("%s: Process %*d \t Access [Page %4d, Offset %#05x] (%#010x)\n",
               current_ref,
               MAX_PID_LEN, pid,
               GET_PAGE(virtual_addr), GET_OFFSET(virtual_addr), virtual_addr);

        access_page(pid, mode, virtual_addr, &physical_addr);

        printf("%s: Process %*d \t Access [Page %4d, Offset %#05x] (%#010x) --> (%#010x) [Frame %4d, Offset %#05x]\n",
               current_ref,
               MAX_PID_LEN, pid,
               GET_PAGE(virtual_addr), GET_OFFSET(virtual_addr), virtual_addr,
               physical_addr, GET_FRAME(physical_addr), GET_OFFSET(physical_addr));
    }

    gettimeofday(&stats.end, NULL);
    display_stats();

    /*
     * Cleanup
     */
    fclose(access_fd);

    if( NULL != current_ref ) {
        free(current_ref);
        current_ref = NULL;
    }

    free_ram();
    free_page_table();
    free_tlb();
    free_cache();

    return 0;
}

/*********************************************************/
static int parse_args(int argc, char **argv,
                      char **access_file,
                      int *n_pages, int *n_frames,
                      int *n_cache, int *n_tlb)
{
    if( argc < 2 || argc > 6 ) {
        fprintf(stderr, "Usage: InputFile [NumPages] [NumFrames] [SizeCache] [SizeTLB]\n");
        fprintf(stderr, " - A negative or 0 value for any of the optional arguments implies default\n");
        fprintf(stderr, "Defaults\n");
        fprintf(stderr, "  [NumPages]  = %10d\n", DEFAULT_NUM_PAGES);
        fprintf(stderr, "  [NumFrames] = %10d\n", DEFAULT_NUM_FRAMES);
        fprintf(stderr, "  [SizeCache] = %10d\n", DEFAULT_CACHE_ENTRIES);
        fprintf(stderr, "  [SizeTLB]   = %10d\n", DEFAULT_TLB_ENTRIES);
        return -1;
    }

    *access_file = strdup(argv[1]);
    printf("Ref. File   : %10s\n", *access_file);

    /*
     * Pages
     */
    if( argc > 2 ) {
        *n_pages  = atoi(argv[2]);
    } else {
        *n_pages  = DEFAULT_NUM_PAGES;
    }
    if( *n_pages <= 0 ) {
        *n_pages  = DEFAULT_NUM_PAGES;
    }
    printf("Num. Pages  : %10d ", *n_pages);
    pretty_print(*n_pages, PAGE_SIZE);
    if( DEFAULT_NUM_PAGES == *n_pages ) {
        printf(" (Default)");
    }
    printf("\n");

    /*
     * Frames
     */
    if( argc > 3 ) {
        *n_frames  = atoi(argv[3]);
    } else {
        *n_frames  = DEFAULT_NUM_FRAMES;
    }
    if( *n_frames <= 0 ) {
        *n_frames  = DEFAULT_NUM_FRAMES;
    }
    printf("Num. Frames : %10d ", *n_frames);
    pretty_print(*n_frames, PAGE_SIZE);
    if( DEFAULT_NUM_FRAMES == *n_frames ) {
        printf(" (Default)");
    }
    printf("\n");

    /*
     * Cache Size
     */
    if( argc > 4 ) {
        *n_cache  = atoi(argv[4]);
    } else {
        *n_cache  = DEFAULT_CACHE_ENTRIES;
    }
    if( *n_cache <= 0 ) {
        *n_cache  = DEFAULT_CACHE_ENTRIES;
    }
    printf("Size Cache  : %10d ", *n_cache);
    pretty_print(*n_cache, PAGE_SIZE);
    if( DEFAULT_CACHE_ENTRIES == *n_cache ) {
        printf(" (Default)");
    }
    printf("\n");

    /*
     * TLB Size
     */
    if( argc > 5 ) {
        *n_tlb  = atoi(argv[5]);
    } else {
        *n_tlb  = DEFAULT_TLB_ENTRIES;
    }
    if( *n_tlb <= 0 ) {
        *n_tlb  = DEFAULT_TLB_ENTRIES;
    }
    printf("Size TLB    : %10d ", *n_tlb);
    if( DEFAULT_TLB_ENTRIES == *n_tlb ) {
        printf("        (Default)");
    }
    printf("\n");

    printf("------------------------------------\n");

    return 0;
}

static int extract_args(char *ref, pid_t *pid, char *mode, addr_t *address) {
    sscanf(ref, "%d, %c, %i\n", pid, mode, address);
    return 0;
}

/*********************************************************
 * Core Memory Management Operation
 *********************************************************/
static int access_page(pid_t pid, char mode, addr_t address, addr_t *physical_addr) {
    frame_t frame;
    int ret;

    /*
     * Check for a context switch
     */
    if(last_pid != pid ) {
        last_pid = pid;
        stats.num_context_switch++;
    }

    /*
     * Check for valid page reference
     */
    if( 0 != check_address(address) ) {
        printf("%s: Fault! Invalid Address Reference!\n", current_ref);
        stats.num_errors++;
        return -1;
    }

    /*
     * Check the Cache
     * - Success: Load page!
     * - Failure: Check TLB
     * Return:
     *  > 0: Resolved in cache
     *  = 0: Not resolved in cache
     *  < 0: Error!
     */
    ret = check_cache(pid, mode, address, &frame);
    if( ret < 0 ) {
        fprintf(stderr, "Error: Cache lookup failed!\n");
        stats.num_errors++;
        return -1;
    }
    else if( ret > 0 ) {
        printf("%s: Cache Hit!\n", current_ref);
        stats.cache_hit++;
        *physical_addr = frame*PAGE_SIZE + GET_OFFSET(address);
        return 0;
    }
    stats.cache_miss++;
    printf("%s: Cache Miss...\n", current_ref);

    /*
     * Check the TLB
     * - Success: Access page from RAM, update cache
     * - Failure: Look in Page Table
     *
     * Return:
     *  > 0: Resolved in TLB
     *  = 0: Not resolved in TLB
     *  < 0: Error!
     */
    ret = check_tlb(pid, mode, address, &frame);
    if( ret < 0 ) {
        fprintf(stderr, "Error: TLB lookup failed!\n");
        stats.num_errors++;
        return -1;
    }
    else if( ret > 0 ) {
        printf("%s: TLB Hit!\n", current_ref);
        stats.tlb_hit++;
        *physical_addr = frame*PAGE_SIZE + GET_OFFSET(address);
        return 0;
    }
    stats.tlb_miss++;
    printf("%s: TLB Miss...\n", current_ref);

    /*
     * Check the Page Table
     * - In RAM:
     *   - Access Page from RAM
     *   - Update TLB, Cache
     * - In Swap:
     *   - Find a frame to replace
     *   - Page-Fault
     *   - Update Page Table
     *   - Update TLB, Cache
     */
    ret = check_page_table(pid, mode, address, &frame);
    if( ret < 0 ) {
        fprintf(stderr, "Error: Page Table lookup failed!\n");
        stats.num_errors++;
        return -1;
    }
    else if( ret > 0 ) {
        *physical_addr = frame*PAGE_SIZE + GET_OFFSET(address);
        return 0;
    }

    fprintf(stderr, "%s: Error: Page Table Miss! This should never happen\n", current_ref);
    exit(-1);

    return -1;
}
