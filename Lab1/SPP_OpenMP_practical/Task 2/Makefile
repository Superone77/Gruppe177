CC=gcc
CXX=g++
CFLAGS=-O3 -fopenmp -std=c99
CXXFLAGS=-O3 -fopenmp
EXECS=matmul_symmetric

all: $(EXECS)

matmul_symmetric: symmMatVecMult.c
	$(CC) -o $@ $<  $(CFLAGS)

clean:
	rm -f $(EXECS) *.o
