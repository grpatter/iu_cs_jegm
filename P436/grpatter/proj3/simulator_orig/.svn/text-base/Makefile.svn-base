#
# Josh Hursey
# P436: Introducation to Operating Systems
# Assignment 3: Memory Management Simulator
#
#####################################################################
#
# Type "make" or "make all" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=g++
CFLAGS=-Wall -g

all: simulator

support: support.c support.h
	$(CC) -c -o support.o $(CFLAGS) support.c

stats: stats.c simulator.h
	$(CC) -c -o stats.o $(CFLAGS) stats.c

ram: ram.c simulator.h
	$(CC) -c -o ram.o $(CFLAGS) ram.c

pagetable: pagetable.c simulator.h
	$(CC) -c -o pagetable.o $(CFLAGS) pagetable.c

tlb: tlb.c simulator.h
	$(CC) -c -o tlb.o $(CFLAGS) tlb.c

cache: cache.c simulator.h
	$(CC) -c -o cache.o $(CFLAGS) cache.c

simulator: simulator.c simulator.h cache.o tlb.o pagetable.o ram.o support.o stats.o
	$(CC) -o simulator $(CFLAGS) simulator.c cache.o tlb.o pagetable.o ram.o support.o stats.o

clean:
	$(RM) simulator *.o
	$(RM) -rf *.dSYM
