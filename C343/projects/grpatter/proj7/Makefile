# Project 7, C343: Data Structures, Indiana University
# File: Makefile
# Name: Greg Patterson, grpatter
# Date: 11/03/2009
CFLAGS+=-std=c99 # Use the C99 standard
CFLAGS+=-W -Wall -pedantic # Turn on all compile-time warnings
CFLAGS+=-g # Enable debugging

CPPFLAGS+=-DMEMORY_SIZE=1024 # Define the MEMORY_SIZE of the heap

LDFLAGS+=-W -Wall -pedantic # Turn on all link-time warnings

PROGRAMS=cats
OBJECTS=cats.o utils.o
SOURCES=cats.c cats.h utils.h utils.c Makefile
TAR_FILE=project7.tar.gz

# Standard targets
.PHONY: all clean
all: $(PROGRAMS)
tar: $(SOURCES)
	-rm -f $(TAR_FILE)
	tar -zcf $(TAR_FILE) $(SOURCES) ReadMe.txt 
clean:
	-rm -f $(PROGRAMS)
	-rm -f $(OBJECTS)
	-rm -f $(TAR_FILE)

# Executables
cats: cats.o utils.o
	gcc $(LDFLAGS) $^ -o $@ -lm

# Object files
cats.o: cats.c cats.h utils.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
utils.o: utils.c utils.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
