#!/usr/bin/env perl

#
# P436: Introducation to Operating Systems
# Assignment 3: Memory Management Simulator
#
# Script to assist with generating reference strings for the Memory Management
# Simulator.
#

use strict;

my $NUM_PROCESSES = 10;
my $AVG_NUM_ACCESSES_PER_CONTEXT_SWITCH = 1000;
my $AVG_FOOTPRINT = 500;
my $MAX_TRACE_LENGTH = 1000000;

my $PAGE_SIZE = 4096;
my $WORD_SIZE  = 4;
my $WORD_ALIGN = $PAGE_SIZE*1024*1024;
my $ALPHA = 0.6;
my $READ_DOMINANCE = 3; # 4-1 reads to writes.

if( ($AVG_FOOTPRINT/2) > $WORD_ALIGN ) {
    print "Error: Footprint can't be bigger than 4gb\n";
    exit -1;
}

my $num_accesses = 0;
my %footprint = (); #random(0, 2*$AVG_FOOTPRINT);
#my %num_pages = ();

my $last_proc = -1;

while($num_accesses < $MAX_TRACE_LENGTH) {
    my $proc_id;
    my $page_id;
    my $rw;
    my $acc;
    my $i;
    my $addr;

    #
    # Choose a process
    #
    do {
        $proc_id = random(0, $NUM_PROCESSES);
    } while($proc_id == $last_proc);
    $last_proc = $proc_id;

    #
    # Determine the number of page accesses before a context switch
    #
    $acc = random(0, 2*$AVG_NUM_ACCESSES_PER_CONTEXT_SWITCH);
    for( $i=0; $i < $acc; ++$i ) {
        #
        # Keep track of the footprint of each process
        #
        if (!exists $footprint{$proc_id}) {
            $footprint{$proc_id} = random(1, 2*$AVG_FOOTPRINT);
            #$num_pages{$proc_id} = 0;
        }

        #
        # Determine the page
        #
        $page_id = int(pareto(1.0, $ALPHA));
        if($page_id > $footprint{$proc_id}) {
            next;
        }

        #
        # Create an address within the page (word aligned)
        #
        $addr = random(0, ($PAGE_SIZE-1));
        $addr = ($page_id*$PAGE_SIZE) + $addr;
        $addr = $addr & ($WORD_ALIGN-$WORD_SIZE);

        #
        # Determine type of access: R/W
        #
        $rw = random(0, $READ_DOMINANCE);
        if ($rw>0) {
            $rw = "R";
        } else {
            $rw = "W";
        }

        #
        # Display the access
        #
        print("$proc_id, $rw, ");
        #print("($page_id) ");
        printf("0x%08lx\n", $addr);

        $num_accesses++;
        if( $num_accesses >= $MAX_TRACE_LENGTH ) {
            last;
        }
    }
}

exit;

sub random() {
    my ($min, $max) = @_;
    return int( rand( $max-$min+1 ) ) + $min;
}

sub pareto() {
    my ($b, $alpha) = @_;
    return ($b / ( (1.0-rand(1.0))**(1.0/$alpha) ) );
}
