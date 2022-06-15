CC=g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_
all: bin/main
bin/main: ./src/main.cpp  ./bin/Objects.o ./src/Objects.cpp 
	$(CC) $(FLAG) ./src/main.cpp ./bin/Objects.o -o ./bin/main

bin/Objects.o: ./src/Objects.cpp ./lib/Objects.h
	$(CC) $(FLAG) -c ./src/Objects.cpp -o ./bin/Objects.o
clean:
		rm -rf *.o  bin/*


