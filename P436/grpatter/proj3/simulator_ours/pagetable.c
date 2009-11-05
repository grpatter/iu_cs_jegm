/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"

proc_page_table_ref_t *sys_proc_table = NULL;

static int num_pages  = 0;
static int num_pids   = 0;

/*
 * Clear the page table of all entries
 */
static void flush_page_table(int index);

/*
 * Create a new page table for a specific process
 */
static int  create_new_page_table(pid_t pid);

/*
 * Display the page table contents
 */
static void display_page_table(int index);

/*
 * Find a page table entry
 */
static int find_page_table_entry(addr_t address, int index, page_table_leaf_t **page_entry);

/*********************************************************
 * Page Table
 *********************************************************/
int allocate_page_table(int n) {
    num_pages = n;
    return 0;
}

int free_page_table(void) {
    int p;

    for( p = 0; p < num_pids; ++p ) {
        /* Free the core page table */
        free(sys_proc_table[p].page_table->pages);
        sys_proc_table[p].page_table->pages = NULL;

        /* Free the table structure */
        free(sys_proc_table[p].page_table);
        sys_proc_table[p].page_table = NULL;
    }

    /* Free the system table */
    free(sys_proc_table);
    sys_proc_table = NULL;

    return 0;
}

int check_address(int addr) {
    /* If beyond the number of pages */
    if( num_pages <= GET_PAGE(addr) ) {
        /*printf("%s: Fault! Invalid Page Reference (%d)!\n", current_ref, GET_PAGE(addr));*/
        return -1;
    }

    /* If offset is larger than a page */
    if( PAGE_SIZE <= GET_OFFSET(addr) ) {
        printf("%s: Fault! Invalid Offset (%d)!\n", current_ref, GET_OFFSET(addr));
        return -1;
    }

    return 0;
}

int check_page_table(pid_t pid, char mode, addr_t address, frame_t *frame) {
    bool found = false;
    int i, index;
    page_table_leaf_t *pt_entry = NULL;

    /*
     * Simulate time needed to access System Page Table
     */
    nanosleep(&ACCESS_TIME_RAM, NULL);

    /*
     * Check the System Page Table to find the entry for this pid
     */
    for( i = 0; i < num_pids; ++i ) {
        if( sys_proc_table[i].pid == pid ) {
            found = true;
            index = i;
            break;
        }
    }

    /*
     * If not found, then create a spot for it.
     */
    if( !found ) {
        printf("%s: New Process (%*d), Creating Page Table\n", current_ref,
               MAX_PID_LEN, pid);

        index = num_pids;
        if( 0 != create_new_page_table(pid) ) {
            return -1;
        }
    }

    /*
     * Find the page table entry
     */
    find_page_table_entry(address, index, &pt_entry);

    /*
     * If the page is not valid, then it is not resident in RAM, so we need
     * to swap it in from disk.
     */
	bool hadPF = false;
    if( !pt_entry->valid ) {
        /*
         * Page Fault the page into RAM
         */
		hadPF = true;
        if( 0 > page_fault(pt_entry) ) {
            return -1;
        }
    } else {
        stats.pt_hit++;
    }
	
	/*
	 * Update RAM Access
	 * -Force RAM to acknowledge and perform any access updates
	 *  after we determine the frame it is in for ease of update.
	 */
	if(!hadPF){
		page_access_ram(*frame);//
	}
	
    /*
     * Update Cache
     * - Write through cache will update RAM for us
     */
    add_to_cache(pt_entry->pid, pt_entry->page, pt_entry->frame, mode);

    /*
     * Update the TLB
     */
    add_to_tlb(pt_entry->page, pt_entry->frame, pt_entry->pid);

    *frame = pt_entry->frame;	

    return 1;
}

/*******************************/
static void display_page_table(int index) {
    int i;

    printf("PID,  Page, V \n");
    printf("-------------------\n");
    for( i = 0; i < num_pages; ++i ) {
        printf("%3d, %5d, %c\n",
               sys_proc_table[index].page_table->pages[i].pid,
               sys_proc_table[index].page_table->pages[i].page,
               (sys_proc_table[index].page_table->pages[i].valid ? 'T' : 'F')
               );
    }
}

static void flush_page_table(int index) {
    int i;

    for( i = 0; i < num_pages; ++i ) {
        sys_proc_table[index].page_table->pages[i].pid   = sys_proc_table[index].pid;
        sys_proc_table[index].page_table->pages[i].page  = i;
        sys_proc_table[index].page_table->pages[i].frame = 0;
        sys_proc_table[index].page_table->pages[i].valid = 0;
    }
}

static int create_new_page_table(pid_t pid) {
    int index = num_pids;
    num_pids++;

    stats.num_procs = num_pids;

    /* Grow the system page table reference */
    sys_proc_table = (proc_page_table_ref_t *)realloc(sys_proc_table, (sizeof(proc_page_table_ref_t) * (num_pids)) );
    if( NULL == sys_proc_table ) {
        fprintf(stderr, "Error: Unable to grow the System Page Table\n");
        return -1;
    }

    /* Create the entry for this PID */
    sys_proc_table[index].pid = pid;
    sys_proc_table[index].page_table = (page_table_t*)malloc(sizeof(page_table_t));
    if( NULL == sys_proc_table[index].page_table ) {
        fprintf(stderr, "Error: Unable to create a page table for process %d\n", pid);
        return -1;
    }

    /* Create the core page table */
    sys_proc_table[index].page_table->pages = (page_table_leaf_t*)malloc(sizeof(page_table_leaf_t) * num_pages);
    if( NULL == sys_proc_table[index].page_table->pages ) {
        fprintf(stderr, "Error: Unable to create a page table for process %d\n", pid);
        return -1;
    }

    /* Initialize the page table for this process to empty */
    flush_page_table(index);

    return 0;
}

int find_page_table_entry(addr_t address, int index, page_table_leaf_t **page_entry) {
    /*
     * Simulate time needed to bring in page table from RAM
     */
    nanosleep(&ACCESS_TIME_RAM, NULL);

    /*
     * Single level page table
     */
    *page_entry = &(sys_proc_table[index].page_table->pages[GET_PAGE(address)]);

    return 0;
}
