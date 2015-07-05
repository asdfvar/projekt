CPPC = g++
INC = -I..
LIBS = -lkernels
LIBPATH = -L..
FLAGS = -O3 -Wall

EXE = test

OBJ = test.o

all: $(EXE)

clean:
	rm $(OBJ) $(EXE)

$(EXE): $(OBJ)
	cd .. && $(MAKE)
	$(CPPC) $(OBJ) $(INC) $(LIBPATH) $(LIBS) -o $(EXE)

test.o: test.cpp
	$(CPPC) $^ $(INC) $(LIBPATH) $(LIBS) -c
