CC=gcc
CXX=g++
CFLAGS=-O3 -std=c99
CXXFLAGS=-O3
EXECS=heated-plate

all: $(EXECS)

heated-plate: heated-plate-parallel.cpp
	mpic++ -o $(EXECS)  heated-plate-parallel.cpp $(CXXFLAGS)
clean:
	rm -f $(EXECS) *.o