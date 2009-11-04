
CFLAGS+=-std=c99 # Use the C99 standard
CFLAGS+=-W -Wall -pedantic # Turn on all compile-time warnings
CFLAGS+=-g # Enable debugging

CPPFLAGS+=-DMEMORY_SIZE=1024 # Define the MEMORY_SIZE of the heap

LDFLAGS+=-W -Wall -pedantic # Turn on all link-time warnings

PROGRAMS=cats
OBJECTS=cats.o
SOURCES=cats.c Makefile
TAR_FILE=project7.tar

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
cats: cats.o
	gcc $(LDFLAGS) $^ -o $@ -lm

# Object files
cats.o: cats.c
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@
