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
	-rm -rf cats cats.o proj8.tar.gz
tar: proj8.tar.gz ReadMe.txt 

# Executables
cats: cats.o utils.o
	gcc $(LDFLAGS) $^ -o $@

# Object files
cats.o: cats.c utils.h
	gcc $(CFLAGS) -c $< -o $@
utils.o: utils.c utils.h
	gcc $(CFLAGS) -c $< -o $@

# Tar file
proj8.tar.gz: Makefile cats.c
	tar -zcvf $@ $^
