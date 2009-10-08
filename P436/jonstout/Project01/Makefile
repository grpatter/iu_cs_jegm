# Jonathan M Stout and Munir Khabibulev

CC = g++
CFLAG = -pthread

all:
	make philosopher deadPhilosopher

philosopher:
	$(CC) $(CFLAG) nonDeadPhilosopher.cpp -o philosopher.o
deadPhilosopher:
	$(CC) $(CFLAG) deadPhilosopher.cpp -o deadPhilosopher.o
workerProblem:
	$(CC) $(CFLAG) workerProblem.cpp
