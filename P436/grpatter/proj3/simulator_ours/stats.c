/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include "simulator.h"
#include <list>

//----------------------------------------------------------------------------
stat_entry_t stats;
//----------------------------------------------------------------------------

/*
 * EAT = Effective Access Time
 */
static double eat_total(void);
static double eat_cache(void);
static double eat_tlb(void);
static double eat_ram(void);

void display_stats(void) {

    stats.cache_hit_ratio = 0.0;

    stats.tlb_hit_ratio   = 0.0;

    stats.page_fault_rate = 0.0;

    stats.swap_out_ratio  = 0.0;

	//----------------------------------------------------------------------------	
	//determine real # of pids
	std::list<pid_t> pid_c;
	for(int i = 0; i < DEFAULT_NUM_PAGES; i++){
		if(stats.pid_list[i] != -1){
			pid_c.push_back(stats.pid_list[i]);
		}
	}
	pid_c.sort();
	pid_c.unique();
	stats.num_procs = pid_c.size();
	
	//----------------------------------------------------------------------------

    printf("------------------------------------\n");
    printf("General:\n");
    printf("   Num. Procs      : %10d\n", stats.num_procs);
    printf("   Context Switch  : %10d\n", stats.num_context_switch);
    printf("   Num. Errors     : %10d\n", stats.num_errors);
    printf("   Time            : %10.3f s\n", timeval_to_double(get_timeval_diff_as_timeval(stats.start, stats.end)));
    printf("   EAT (Total)     : %10.3f ms\n", eat_total());

    printf("Cache: (%5d Entries = ", stats.cache_size);
    pretty_print(stats.cache_size, PAGE_SIZE);
    printf(")\n");
    printf("   Miss            : %10d\n", stats.cache_miss);
    printf("   Hit             : %10d\n", stats.cache_hit);
    printf("   Hit Ratio       : %10.1f %c\n", (stats.cache_hit_ratio * 100), '%');
    printf("   EAT (Cache)     : %10.1f ns\n", eat_cache());

    printf("TLB  : (%5d Entries = ", stats.tlb_size);
    pretty_print(stats.tlb_size, PAGE_SIZE);
    printf(")\n");
    printf("   Miss            : %10d\n", stats.tlb_miss);
    printf("   Hit             : %10d\n", stats.tlb_hit);
    printf("   Hit Ratio       : %10.1f %c\n", (stats.tlb_hit_ratio * 100), '%');
    printf("   EAT (TLB)       : %10.1f ns\n", eat_tlb());

    printf("Page Table:\n");
    printf("   Virtual Address : (%6d Entries = ", stats.num_pages);
    pretty_print(stats.num_pages, PAGE_SIZE);
    printf(")\n");
    printf("   Physical Memory : (%6d Entries = ", stats.num_frames);
    pretty_print(stats.num_frames, PAGE_SIZE);
    printf(")\n");
    printf("   Hits            : %10d\n", stats.pt_hit);
    printf("   Page Fault      : %10d\n", stats.page_faults);
    printf("   Swap Out        : %10d\n", stats.swap_out);
    printf("   Swap In         : %10d\n", stats.swap_in);
    printf("   Page Fault Rate : %10.1f %c\n", (stats.page_fault_rate * 100), '%');
    printf("   Swap Out Ratio  : %10.1f %c\n", (stats.swap_out_ratio * 100), '%');
    printf("   EAT (Page)      : %10.1f ms\n", eat_ram());
    printf("------------------------------------\n");
}

void clear_stats(void) {
    /* Cache Stats */
    stats.cache_miss = 0;
    stats.cache_hit  = 0;
    stats.cache_hit_ratio = 0.0;

    /* TLB Stats */
    stats.tlb_miss = 0;
    stats.tlb_hit  = 0;
    stats.tlb_hit_ratio = 0.0;

    /* Page Table Stats */
    stats.pt_hit      = 0;
    stats.page_faults = 0;
    stats.page_fault_rate = 0.0;

    stats.swap_out    = 0;
    stats.swap_in     = 0;
    stats.swap_out_ratio = 0.0;

    /* Processes */
    stats.num_procs   = 0;
    stats.num_context_switch = 0;

    /* Correctness */
    stats.num_errors  = 0;
	
	/* pid list */
	for(int i = 0; i < DEFAULT_NUM_PAGES; i++){
	stats.pid_list[i] = -1;
	}
}

void pretty_print(int32_t value, int32_t mult) {
    int32_t x = value;
    int32_t m = mult;
    char d[3] = {' ', 'B','\0'};
    int order = 0;

    while (m > 1024 ) {
        m /= 1024;
        order++;
    }

    x *= m;
    while (x > 1024 ) {
        x /= 1024;
        order++;
    }

    if(      order == 1 ) {  d[0] = 'K'; }
    else if( order == 2 ) {  d[0] = 'M'; }
    else if( order == 3 ) {  d[0] = 'G'; }
    else if( order == 4 ) {  d[0] = 'T'; }
    else                  {  d[0] = '-'; }

    printf("%4d %s", x, d);
}

/*********************************************/
static double eat_total(void) {
    return 0.0;
}

static double eat_cache(void) {
    return 0.0;
}

static double eat_tlb(void) {
    return 0.0;
}

static double eat_ram(void) {
    return 0.0;
}
