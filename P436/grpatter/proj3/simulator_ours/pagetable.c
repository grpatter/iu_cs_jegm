/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"

proc_page_table_ref_t *sys_proc_table = NULL;
page_directory page_dir;

static int num_pages  = 0;
static int num_tables  = 0;//
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

/*
** 
*/

/*********************************************************
 * Page Table
 *********************************************************/
int allocate_page_table(int n) {
    num_pages = n;
    return 0;
}

int allocate_page_directory(int n){//
	num_tables = n;
	//null shit out please
	page_dir.dir_e = (dir_entry*)malloc(sizeof(dir_entry)*1024);
	printf("Nulling page_dir values...\n");
	for(int i = 0; i < 1024; ++i){
		//printf("Null iteration:%d...\n",i);
		page_dir.dir_e[i].exists = false;
		page_dir.dir_e[i].table_ref = NULL;
	}
	return 0;
}

int free_page_dir(void){
	for(int i = 0; i < 1024; ++i){
		if(page_dir.dir_e[i].exists){
			sys_proc_table = page_dir.dir_e[i].table_ref;
			free_page_table();
		}
	}
	free(page_dir.dir_e);
	page_dir.dir_e = NULL;
}

int free_page_table(void) {
    int p;

    //for( p = 0; p < num_pids; ++p ) {
    for( p = 0; p < 1; ++p ) {
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

//----------------------------------------------------------------------------

int check_address(int addr) {
    /* If beyond the number of pages */
    //if( num_pages <= GET_PAGE(addr) ) {
    if( num_pages <= GET_PAGE_E(GET_PAGE(addr))) {
	//if(VERBOSE){printf("HEX address is:(%#010x)...",(unsigned)addr);}
        printf("%s: Fault! Invalid Page Reference (%d)!\n", current_ref, GET_PAGE(addr));
	//if(VERBOSE){printf("GET_PAGE:(%#010x)...PAGE_IND:(%#010x)...PAGE_E:(%#010x)...\n",GET_PAGE(addr), GET_PAGE_IND(addr), GET_PAGE_E(GET_PAGE(addr)));}
        return -1;
    }

    /* If offset is larger than a page */
    if( PAGE_SIZE <= GET_OFFSET(addr) ) {
        printf("%s: Fault! Invalid Offset (%d)!\n", current_ref, GET_OFFSET(addr));
        return -1;
    }

    return 0;
}
//----------------------------------------------------------------------------


int check_page_dir(pid_t pid, char mode, addr_t address, frame_t *frame){
//get address offset
//look for existing table entry
//if(exist), give it to check_page_table
//else, create (make sure to create sys_proc_table), send fault, try again
addr_t dir_offset = GET_PAGE_IND(address);
bool found = false;
nanosleep(&ACCESS_TIME_RAM, NULL);
	//check directory for table entry
	if(page_dir.dir_e[dir_offset].exists){//we have an entry, proceed with lookup
		sys_proc_table = page_dir.dir_e[dir_offset].table_ref;
		int check = check_page_table(pid, mode, address, frame);
		printf("check_page_dir entry exists and resulted in check=%d...\n",check);
	}else{//we need to create it
		dir_entry de = create_dir_entry(pid);
		page_dir.dir_e[dir_offset] = de;
		printf("check_page_dir entry did not exist and was created at offset=%#010x...\n",dir_offset);
	}
	sys_proc_table = NULL;
}

dir_entry create_dir_entry(pid_t pid){
	dir_entry de;
	de.exists = true;
	create_new_page_table(pid);
	de.table_ref = sys_proc_table;
	return de;
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
     * Update Cache
     * - Write through cache will update RAM for us
     */
    add_to_cache(pt_entry->pid, pt_entry->page, pt_entry->frame, mode);

    /*
     * Update the TLB
     */
    add_to_tlb(pt_entry->page, pt_entry->frame, pt_entry->pid);

    *frame = pt_entry->frame;	
	
	/*
	 * Update RAM Access
	 * -Force RAM to acknowledge and perform any access updates
	 *  after we determine the frame it is in for ease of update.
	 */
	if(!hadPF){
		page_access_ram(*frame);//
	}
	
	
    return 1;
}

/*******************************/
static void display_page_table(int index) {
    int i;

	if (VERBOSE) {
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
	if(VERBOSE){printf("start create new page table with pid %d\n",(int)pid);}
    int index = num_pids;
    num_pids++;

    stats.num_procs = num_pids;

	if(VERBOSE){printf("realloc create new page table with pid %d\n",(int)pid);}
    /* Grow the system page table reference */
    sys_proc_table = (proc_page_table_ref_t *)realloc(sys_proc_table, (sizeof(proc_page_table_ref_t) * (num_pids)) );
    if( NULL == sys_proc_table ) {
        fprintf(stderr, "Error: Unable to grow the System Page Table\n");
        return -1;
    }

	if(VERBOSE){printf("pid entry create new page table with pid %d\n",(int)pid);}
    /* Create the entry for this PID */
    sys_proc_table[index].pid = pid;
	if(VERBOSE){printf("pid set to %d\n",(int)sys_proc_table[index].pid);}
    sys_proc_table[index].page_table = (page_table_t*)malloc(sizeof(page_table_t));
	if(VERBOSE){printf("page_table + malloc'd\n");}
    if( NULL == sys_proc_table[index].page_table ) {
        fprintf(stderr, "Error: Unable to create a page table for process %d\n", pid);
        return -1;
    }

	if(VERBOSE){printf("core table in create new page table with pid %d\n",(int)pid);}
    /* Create the core page table */
    sys_proc_table[index].page_table->pages = (page_table_leaf_t*)malloc(sizeof(page_table_leaf_t) * num_pages);
    if( NULL == sys_proc_table[index].page_table->pages ) {
        fprintf(stderr, "Error: Unable to create a page table for process %d\n", pid);
        return -1;
    }

	if(VERBOSE){printf("preflush create new page table with pid %d\n",(int)pid);}
    /* Initialize the page table for this process to empty */
    flush_page_table(index);

	if(VERBOSE){printf("end create new page table with pid %d\n",(int)pid);}
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
	//if(VERBOSE){printf("HEX address is:(%#010x)...",(unsigned)address);}
    //*page_entry = &(sys_proc_table[index].page_table->pages[GET_PAGE(address)]);
    *page_entry = &(sys_proc_table[index].page_table->pages[GET_PAGE_E(GET_PAGE(address))]);
	//if(VERBOSE){printf("GET_PAGE:(%#010x)...PAGE_IND:(%#010x)...PAGE_E:(%#010x)...\n",GET_PAGE(address), GET_PAGE_IND(address), GET_PAGE_E(GET_PAGE(address)));}

    return 0;
}
