# Project 3, C343: Data Structures, Indiana University
# File: Makefile
# Name: Greg Patterson
# Username: grpatter
# Credit: Michael Adams
# Date: 2009 Sept.

CFLAGS+=-std=c99 # Use the C99 standard
CFLAGS+=-W -Wall -pedantic # Turn on all compile-time warnings
CFLAGS+=-g # Enable debugging

CPPFLAGS+=-DMEMORY_SIZE=1024 # Define the MEMORY_SIZE of the heap

LDFLAGS+=-W -Wall -pedantic # Turn on all link-time warnings

PROGRAMS=linkedCalculator
OBJECTS=AVAIL.o linkedStack.o linkedCalculator.o
SOURCES=AVAIL.c linkedStack.c linkedCalculator.c AVAIL.h stack.h Makefile
TAR_FILE=project3.tar

# Standard targets
.PHONY: all clean
all: $(PROGRAMS)
tar: $(SOURCES)
	-rm -f $(TAR_FILE)
	tar -cvf $(TAR_FILE) $(SOURCES)
clean:
	-rm -f $(PROGRAMS)
	-rm -f $(OBJECTS)
	-rm -f $(TAR_FILE)

# Executables
linkedCalculator: AVAIL.o linkedStack.o linkedCalculator.o
	gcc $(LDFLAGS) $^ -o $@ -lm

# Object files
AVAIL.o: AVAIL.c AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
linkedStack.o: linkedStack.c stack.h AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
linkedCalculator.o: linkedCalculator.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
