CC=g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_
all: bin/main

bin/main: ./src/main.cpp  ./obj/Objects.o ./obj/SimulatedAnnealing.o ./src/Objects.cpp  ./src/SimulatedAnnealing.cpp ./bin/Partition.o ./src/Partition.cpp 
	$(CC) $(FLAG) ./src/main.cpp ./obj/Objects.o ./bin/Partition.o ./obj/SimulatedAnnealing.o -o ./bin/main

obj/Objects.o: ./src/Objects.cpp ./lib/Objects.h
	$(CC) $(FLAG) -c ./src/Objects.cpp -o ./obj/Objects.o
bin/Partition.o:  ./src/Partition.cpp ./bin/Objects.o ./src/Objects.cpp
	$(CC) $(FLAG) -c ./src/Partition.cpp ./bin/Objects.o -o ./bin/Partition.o
obj/SimulatedAnnealing.o: ./src/SimulatedAnnealing.cpp ./lib/SimulatedAnnealing.h
	$(CC) $(FLAG) -c ./src/SimulatedAnnealing.cpp -o ./obj/SimulatedAnnealing.o
clean:
	rm -rf *.o  bin/*


