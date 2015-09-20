CPPC=g++

BASE = ../..

INC = -I$(BASE)/RT -I$(BASE)/objects -I$(BASE)/kernels -I$(BASE)/utilities
LIBS = -lraytrace -lshapes -lkernels -pthread -lutils
LIBPATH = -L$(BASE)/RT -L$(BASE)/objects -L$(BASE)/kernels -L$(BASE)/utilities
FLAGS = -std=c++11

OBJ = test.o

EXE = rt

all: $(OBJ)
	$(CPPC) $(FLAGS) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE)

clean:
	rm $(OBJ) $(EXE)

test.o: test.cpp
	$(CPPC) $^ $(INC) -c
