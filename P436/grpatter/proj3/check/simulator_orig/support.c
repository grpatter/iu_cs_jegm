/*
 * Josh Hursey
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 * Small support library timing
 *
 */
#include "support.h"

double timeval_to_double(struct timeval ctime) {
    return (ctime.tv_sec + (ctime.tv_usec/(1.0 + TIME_USEC)));
}

struct timeval get_timeval_diff_as_timeval(struct timeval start, struct timeval end) {
    struct timeval loc_diff;

    /* Check to see if we have to carry a second over */
    if( end.tv_usec < start.tv_usec ) {
        loc_diff.tv_usec = (TIME_USEC - start.tv_usec) + end.tv_usec;
    } else {
        loc_diff.tv_usec = end.tv_usec - start.tv_usec;
    }

    loc_diff.tv_sec = end.tv_sec - start.tv_sec;

    return loc_diff;
}

double get_timeval_diff_as_double(struct timeval start, struct timeval *given_end) {
    struct timeval loc_diff, end;

    if( NULL == given_end ) {
        gettimeofday(&end, NULL);
    } else {
        end.tv_sec  = given_end->tv_sec;
        end.tv_usec = given_end->tv_usec;
    }

    loc_diff = get_timeval_diff_as_timeval(start, end);

    return timeval_to_double(loc_diff);
}
