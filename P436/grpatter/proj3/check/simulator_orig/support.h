/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

/*****************************
 * Defines
 *****************************/
/* Microsecond: 1 second = 1000000 usec */
#define TIME_USEC     1000000

/* Millisecond: 1 second = 1000 msec */
#define TIME_MSEC     1000

/*****************************
 * Function Declarations
 *****************************/
/*
 * Convert 'struct timeval' to double representing time in milliseconds
 *
 * Arguments:
 *   ctime = Time structure to convert
 *
 * Returns:
 *   Time in milliseconds
 */
double timeval_to_double(struct timeval ctime);

/*
 * Take the difference between two timeval structures
 *
 * Arguments:
 *   start  = Start time (must be smaller than End time)
 *   end    = End time (must be larger than Start time)
 *
 * Returns:
 *   timeval structure containing the difference between the two times.
 */
struct timeval get_timeval_diff_as_timeval(struct timeval start, struct timeval end);

/*
 * Take the difference between two timeval structures
 *
 * Arguments:
 *   start  = Start time (must be smaller than End time)
 *   end    = Reference to End time (must be larger than Start time or NULL)
 *            If end == NULL, then the current gettimeofday() value will be used.
 *
 * Returns:
 *   Time in milliseconds
 */
double get_timeval_diff_as_double(struct timeval start, struct timeval *given_end);
