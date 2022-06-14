CC=g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_


bin/sa: src/Simulated_Annealing.cpp
	$(CC) $(OPTFLAGS) $< -o ./bin/sa

clean:
		rm -rf *.o lib/*.a lib/*.o bin/*


