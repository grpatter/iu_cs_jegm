# File: Makefile
# Name: Jonathan M. Stout
# User: jonstout
# Date: 09/17/2009

CFLAGS= -std=c99

.PHONY: clean all

all:
	make arrayCalculator linkedCalculator stupidCalculator

clean:
	rm -rf *.o linkedCalculator arrayCalculator queueCalculator
cleanAll:
	rm -rf *.o *.*~ linkedCalculator arrayCalculator queueCalculator

# Stupid Calculator
stupidCalculator: stupidCalc.o stupidStack.o queue.o AVAIL.o
	gcc $(CFLAGS) stupidCalc.o stupidStack.o queue.o AVAIL.o -o stupidCalculator
stupidCalc.o: stupidCalculator.c stack.h
	gcc $(CFLAGS) -c stupidCalculator.c -o stupidCalc.o
stupidStack.o: stupidStack.c stack.h queue.h
	gcc $(CFLAGS) -c stupidStack.c -o stupidStack.o
AVAIL.o: AVAIL.c AVAIL.h
	gcc $(CFLAGS) -c AVAIL.c -DMEMORY_SIZE=2048 -o AVAIL.o
queue.o: queue.c queue.h AVAIL.h
	gcc $(CFLAGS) -c queue.c -o queue.o


# Linked Calculator
linkedCalculator: linkedCalc.o linkedStack.o AVAIL.o
	gcc $(CFLAGS) linkedCalc.o linkedStack.o AVAIL.o -o linkedCalculator
linkedCalc.o: linkedCalculator.c stack.h
	gcc $(CFLAGS) -c linkedCalculator.c -o linkedCalc.o
linkedStack.o: linkedStack.c stack.h
	gcc $(CFLAGS) -c linkedStack.c -o linkedStack.o
#AVAIL.o: AVAIL.c AVAIL.h
#	gcc $(CFLAGS) -c AVAIL.c -DMEMORY_SIZE=2048 -o AVAIL.o

# Array Calculator
arrayCalculator: arrayCalc.o arrayStack.o
	gcc $(CFLAGS) arrayCalc.o arrayStack.o -o arrayCalculator
arrayCalc.o: seqCalculator.c stack.h
	gcc $(CFLAGS) -c seqCalculator.c -o arrayCalc.o
arrayStack.o: seqStack.c stack.h
	gcc $(CFLAGS) -c seqStack.c -o arrayStack.o
