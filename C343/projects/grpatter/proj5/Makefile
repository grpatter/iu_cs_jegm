# Project 5, C343: Data Structures, Indiana University
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

PROGRAMS=familyTrees
OBJECTS=familyTrees.o
SOURCES=familyTrees.c tree_node.h Makefile README.txt
TAR_FILE=project5.tar

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
familyTrees: familyTrees.o
	gcc $(LDFLAGS) $^ -o $@ -lm

# Object files
familyTrees.o: familyTrees.c tree_node.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
