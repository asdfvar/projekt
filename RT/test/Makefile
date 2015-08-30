CPPC=g++

BASE = ../..

INC = -I$(BASE)/RT -I$(BASE)/objects -I$(BASE)/kernels

LIBS = -lraytrace -lshapes -lkernels
LIBPATH = -L$(BASE)/RT -L$(BASE)/objects -L$(BASE)/kernels

OBJ = test.o

EXE = rt

all: $(OBJ)
	$(CPPC) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE)

clean:
	rm $(OBJ) $(EXE)

test.o: test.cpp
	$(CPPC) $^ $(INC) -c
