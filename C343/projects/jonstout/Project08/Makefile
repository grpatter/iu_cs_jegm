# To use this makefile, run the Unix command "make <TARGET>", where
# <TARGET> is one of the defined targets: all, cats, or clean.
# The first two compile the project, and clean removes the executable.

# Configuration settings
CFLAGS+=-std=c99 # Use the C99 standard
CFLAGS+=-W -Wall -pedantic # Turn on all compile-time warnings
CFLAGS+=-DNUM_CATS=500 # Set how many cats to handle
CFLAGS+=-g # Turn on debugging
LDFLAGS+=-W -Wall -pedantic # Turn on all link-time warnings

# Standard targets
.PHONY: all clean tar
all: cats
clean:
	-rm -rf cats cats.o proj7.tar.gz
tar: proj7.tar.gz

# Executables
cats: cats.o
	gcc $(LDFLAGS) $^ -o $@

# Object files
cats.o: cats.c
	gcc $(CFLAGS) -c $< -o $@

# Tar file
proj7.tar.gz: Makefile cats.c
	tar -zcvf $@ $^
