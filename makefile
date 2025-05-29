compiler=gcc
flags=-std=gnu11 -march=native -Wall -Wno-missing-braces 

cycles: hamcon_cycles.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConCycles hamcon_cycles.c readGraph/readGraph6.c bitset.h $(flags) -O3

cyclesDebug: hamcon_cycles.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConCyclesDebug hamcon_cycles.c readGraph/readGraph6.c bitset.h $(flags) -fsanitize=address -g -pg

paths: hamcon_paths.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConPaths hamcon_paths.c readGraph/readGraph6.c bitset.h $(flags) -O3

pathsDebug: hamcon_paths.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConPathsDebug hamcon_paths.c readGraph/readGraph6.c bitset.h $(flags) -pg -g -fsanitize=address
	
all: path cycles

all-debug: pathsDebug cyclesDebug

.PHONY: clean
