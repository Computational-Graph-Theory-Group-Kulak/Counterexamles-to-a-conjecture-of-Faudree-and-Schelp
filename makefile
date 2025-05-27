compiler=gcc
flags=-std=gnu11 -march=native -Wall -Wno-missing-braces 

# The 64-bit version of this program is faster but only supports graphs up to 64 vertices.
64bit: hamcon.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamCon hamcon.c readGraph/readGraph6.c bitset.h $(flags) -O3

cycles: hamcon_cycles.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConCycles hamcon_cycles.c readGraph/readGraph6.c bitset.h $(flags) -O3

cyclesDebug: hamcon_cycles.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConCyclesDebug hamcon_cycles.c readGraph/readGraph6.c bitset.h $(flags) -fsanitize=address -g -pg

paths: hamcon_paths.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConPaths hamcon_paths.c readGraph/readGraph6.c bitset.h $(flags) -O3

pathsDebug: hamcon_paths.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConPathsDebug hamcon_paths.c readGraph/readGraph6.c bitset.h $(flags) -pg -g -fsanitize=address
	
gadget: hamconGadgets.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConGadgets hamconGadgets.c readGraph/readGraph6.c bitset.h $(flags) -O3

# The 64-bit version of this program is faster but only supports graphs up to 64 vertices.
gadgetDebug: hamconGadgets.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o hamConGadgets hamconGadgets.c readGraph/readGraph6.c bitset.h $(flags) -pg -g -fsanitize=address

fc: FaudreeSchelp.c readGraph/readGraph6.c bitset.h
	$(compiler) -DUSE_64_BIT -o FaudreeSchelp FaudreeSchelp.c readGraph/readGraph6.c $(flags) -g -pg -fsanitize=address
	
all: path fc cycles

.PHONY: clean
