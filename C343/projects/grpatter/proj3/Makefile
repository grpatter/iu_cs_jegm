# To use this makefile, run the Unix command "make <TARGET>", where
# <TARGET> is one of the defined targets:
#   - all,
#   - tar,
#   - clean,
#   - linkedCalculator,
#   - seqCalculator, or
#   - stupidCalculator.

# Configuration settings
CFLAGS+=-std=c99 # Use the C99 standard
CFLAGS+=-W -Wall -pedantic # Turn on all compile-time warnings
CFLAGS+=-g # Enable debugging

CPPFLAGS+=-DMEMORY_SIZE=1024 # Define the MEMORY_SIZE of the heap

LDFLAGS+=-W -Wall -pedantic # Turn on all link-time warnings

PROGRAMS=linkedCalculator
OBJECTS=AVAIL.o linkedStack.o seqStack.o stupidStack.o queue.o linkedCalculator.o seqCalculator.o stupidCalculator.o
SOURCES=AVAIL.c linkedStack.c seqStack.c stupidStack.c queue.c linkedCalculator.c seqCalculator.c stupidCalculator.c AVAIL.h stack.h queue.h Makefile
TAR_FILE=project2.tar

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
seqCalculator: seqStack.o seqCalculator.o
	gcc $(LDFLAGS) $^ -o $@
stupidCalculator: AVAIL.o stupidStack.o queue.o stupidCalculator.o
	gcc $(LDFLAGS) $^ -o $@

# Object files
AVAIL.o: AVAIL.c AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
linkedStack.o: linkedStack.c stack.h AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
seqStack.o: seqStack.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
stupidStack.o: stupidStack.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
queue.o: queue.c queue.h AVAIL.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
linkedCalculator.o: linkedCalculator.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
seqCalculator.o: seqCalculator.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
stupidCalculator.o: stupidCalculator.c stack.h
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
