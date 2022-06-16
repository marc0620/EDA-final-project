CC=g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_
all: bin/main

bin/main: ./src/main.cpp  ./obj/Objects.o ./obj/SimulatedAnnealing.o ./src/Objects.cpp  ./src/SimulatedAnnealing.cpp ./obj/Partition.o ./src/Partition.cpp ./obj/TerminalPlacing.o ./src/TerminalPlacing.cpp 
	$(CC) $(FLAG) ./src/main.cpp ./obj/Objects.o ./obj/Partition.o ./obj/SimulatedAnnealing.o ./obj/TerminalPlacing.o -o ./bin/main

obj/Objects.o: ./src/Objects.cpp ./lib/Objects.h
	$(CC) $(FLAG) -c ./src/Objects.cpp -o ./obj/Objects.o

obj/Partition.o:  ./src/Partition.cpp ./obj/Objects.o ./src/Objects.cpp
	$(CC) $(FLAG) -c ./src/Partition.cpp ./obj/Objects.o -o ./obj/Partition.o

obj/SimulatedAnnealing.o: ./src/SimulatedAnnealing.cpp ./lib/SimulatedAnnealing.h
	$(CC) $(FLAG) -c ./src/SimulatedAnnealing.cpp -o ./obj/SimulatedAnnealing.o

obj/TerminalPlacing.o: ./src/TerminalPlacing.cpp ./lib/TerminalPlacing.h
	$(CC) $(FLAG) -c ./src/TerminalPlacing.cpp -o ./obj/TerminalPlacing.o

clean:
	rm -rf *.o  bin/* obj/*


