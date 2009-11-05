/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"
#include "pagealgorithms.c"

static int num_frames = 0;
static char *algorithm_str = "random";

/*
 * Free Frame tracking (stack based)
 */
static int next_free_frame = num_frames-1;
static frame_t *free_frame_list = NULL;

frame_ref_t *physical_mem = NULL;

/*
 * Core Page Replacement Algorithm
 */
static int page_replacement_alg(frame_t *victim);

/*
 * A page replacement algorithm that chooses a random page to evict.
 */
static int random_page_replacement_alg(frame_t *victim);

/*
 * Swap a page out to disk
 */
static int swap_out(frame_t victim);

/*
 * Swap a page in from disk
 */
static int swap_in(page_table_leaf_t *page_entry);

/*********************************************************
 * Frame Management
 *********************************************************/
//int allocate_ram(int n) {
int allocate_ram(ram_info_t ram_info) {
    int i;

    //num_frames = n;
    num_frames = ram_info.n_frames;
	algorithm_str = ram_info.algorithm;//
	initAlgorithm(algorithm_str);//

    /* Physical Memory (RAM) */
    physical_mem = (frame_ref_t*)malloc(sizeof(frame_ref_t) * num_frames);
    if( NULL == physical_mem ) {
        fprintf(stderr, "Error: Unable to allocate the Physical Mem data structure\n");
        return -1;
    }
    for( i = 0; i < num_frames; ++i ) {
        physical_mem[i].frame = i;
        physical_mem[i].page_ref = NULL;
    }

    /* Free Frame List */
    free_frame_list = (frame_t*)malloc(sizeof(frame_t) * num_frames);
    if( NULL == free_frame_list ) {
        fprintf(stderr, "Error: Unable to allocate the Free Frame List data structure\n");
        return -1;
    }
    for( i = 0; i < num_frames; ++i ) {
        free_frame_list[i] = i;
    }
    next_free_frame = num_frames-1;

    return 0;
}

int free_ram(void) {
    free(physical_mem);
    physical_mem = NULL;

    free(free_frame_list);
    free_frame_list = NULL;
    return 0;
}

void display_ram(void) {
    int i;

    printf("    Frame, D, R ... Page, PID, V\n");
    printf("------------------------------------\n");
    for(i = 0; i < num_frames; ++i ) {
        if( physical_mem[i].page_ref == NULL ) {
            printf("RAM %5d, %c, %c ... --- FREE ---\n",
                   physical_mem[i].frame,
                   (physical_mem[i].dirty ? 'T' : 'F'),
                   (physical_mem[i].ref   ? 'T' : 'F'));
        } else {
            printf("RAM %5d, %c, %c ... %5d, %5d, %c\n",
                   physical_mem[i].frame,
                   (physical_mem[i].dirty ? 'T' : 'F'),
                   (physical_mem[i].ref   ? 'T' : 'F'),
                   physical_mem[i].page_ref->page,
                   physical_mem[i].page_ref->pid,
                   (physical_mem[i].page_ref->valid ? 'T' : 'F')
                   );
        }
    }
}

int assert_page_in_ram(page_t page) {
    int i;

    for(i = 0; i < num_frames; ++i ) {
        if( NULL != physical_mem[i].page_ref ) {
            if(page == physical_mem[i].page_ref->page ) {
                return 0;
            }
        }
    }

    return -1;
}

int page_fault(page_table_leaf_t *page_entry) {
    frame_t victim = 0;

    stats.page_faults++;

    /*
     * If there are no free frames, then we need to swap something out
     */
    if( next_free_frame < 0 ) {
        /*
         * Pick a victim to swap out
         */
        page_replacement_alg(&victim);

        /*
         * Swap out the page
         */
        swap_out(victim);
    }

    /*
     * Swap-In the page
     */
    swap_in(page_entry);

    return 0;
}

void page_access_ram(frame_t frame){//
	accessActionAlgo(frame);//
}//


/*******************************************************/
static int random_page_replacement_alg(frame_t *victim) {
    /*
     * Random Page Replacement Algorithm
     * - Randomly choose a page to replace
     */
    *victim = rand()%num_frames;

    printf("%s: Page Rep. Alg. Victim = %5d\n", current_ref, *victim);

    return 0;
}

static int page_replacement_alg(frame_t *victim) {
	printf("\nRequested to use Page Eviction Algorithm: %s\n", algorithm_str);//
	int v_res = getVictim(victim);
	if(v_res != -1){
		printf("%s: Page Rep. Alg. Victim = %5d\n", current_ref, *victim);
		return v_res;
	}else{
		return random_page_replacement_alg(victim);
	}	
}

static int swap_out(frame_t victim) {
    printf("%s: Swap Out Page %5d, Frame %5d\n", current_ref,
           physical_mem[victim].page_ref->page,
           victim);

    /*
     * Pull from Cache
     */
    remove_from_cache(physical_mem[victim].page_ref->pid, physical_mem[victim].page_ref->page);

    /*
     * Pull from TLB
     */
    remove_from_tlb(physical_mem[victim].page_ref->page, physical_mem[victim].page_ref->pid);
    
    /*
     * Simulate time to write out to disk (only if page is dirty)
     */
    if( physical_mem[victim].dirty ) {
        nanosleep(&ACCESS_TIME_DISK, NULL);
        stats.swap_out++;
    }

    /*
     * Update the page table of the victim
     */
    physical_mem[victim].page_ref->frame = 0;
    physical_mem[victim].page_ref->valid = false;
    physical_mem[victim].page_ref        = NULL;

    /*
     * Add frame to the free frame list
     */
    next_free_frame++;
    free_frame_list[next_free_frame] = victim;

    return 0;
}

static int swap_in(page_table_leaf_t *page_entry) {
    frame_t fframe = 0;

    stats.swap_in++;

    /*
     * Pull a frame off of the free frame list
     */
    fframe = free_frame_list[next_free_frame];
    free_frame_list[next_free_frame] = -1;
    next_free_frame--;

    printf("%s: Swap In  Page %5d, Frame %5d\n", current_ref,
           page_entry->page,
           fframe);

    /*
     * Simulate time to read in from disk
     */
    nanosleep(&ACCESS_TIME_DISK, NULL);

    /*
     * Update the physical memory table
     */
    physical_mem[fframe].page_ref = page_entry;

    /*
     * Update the Page Table Entry
     */
    page_entry->frame = fframe;
    page_entry->valid = true;
	
	/*
	 * Tell algorithm to update
	 */
	swapInActionAlgo(fframe);//
	
    return 0;
}
