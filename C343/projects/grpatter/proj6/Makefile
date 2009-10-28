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

PROGRAMS=tricalc
OBJECTS=AVAIL.o linkedStack.o tricalc.o
SOURCES=AVAIL.c linkedStack.c tricalc.c AVAIL.h stack.h Makefile
TAR_FILE=project6.tar

# Standard targets
.PHONY: all clean
all: $(PROGRAMS)
tar: $(SOURCES)
	-rm -f $(TAR_FILE)
	tar -cvf $(TAR_FILE) $(SOURCES) ReadMe.txt
clean:
	-rm -f $(PROGRAMS)
	-rm -f $(OBJECTS)
	-rm -f $(TAR_FILE)

# Executables
tricalc: AVAIL.o linkedStack.o tricalc.o
	gcc $(LDFLAGS) $^ -o $@ -lm

# Object files
AVAIL.o: AVAIL.c AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
linkedStack.o: linkedStack.c stack.h AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
tricalc.o: tricalc.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
