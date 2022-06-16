CC=g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_
all: bin/main
bin/main: ./src/main.cpp ./bin/Partition.o ./src/Partition.cpp  ./bin/Objects.o ./src/Objects.cpp
	$(CC) $(FLAG) ./src/main.cpp ./bin/Partition.o ./bin/Objects.o -o ./bin/main
bin/Partition.o:  ./src/Partition.cpp ./bin/Objects.o ./src/Objects.cpp
	$(CC) $(FLAG) -c ./src/Partition.cpp ./bin/Objects.o -o ./bin/Partition.o
bin/Objects.o: ./src/Objects.cpp ./lib/Objects.h
	$(CC) $(FLAG) -c ./src/Objects.cpp -o ./bin/Objects.o
clean:
		rm -rf *.o  bin/*


