# Makefile
# Greg Patterson (grpatter at indiana dot edu)
# 09/21/2009
.PHONY: clean all

all:
	make shell myls
clean:
	rm -rf *.o shell myls

shell: main.cpp
	g++ main.cpp -o shell
	
myls: myls.cpp
	g++ myls.cpp -o myls