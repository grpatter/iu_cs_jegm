# Makefile
# Greg Patterson (grpatter at indiana dot edu)
# 09/21/2009
.PHONY: clean all

all:
	make philDeadlock philNoDeadlock workers
clean:
	rm -rf *.o philDeadlock philNoDeadlock workers

philDeadlock: philDeadlock.cpp
	g++ philDeadlock.cpp -o philDeadlock -pthread
philNoDeadlock: philNoDeadlock.cpp
	g++ philNoDeadlock.cpp -o philNoDeadlock -pthread
workers: workers.cpp
	g++ workers.cpp -o workers -pthread