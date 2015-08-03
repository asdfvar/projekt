CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

BASE = ..
KERNELS = $(BASE)/kernels
RT = $(BASE)/RT

INC = -I$(KERNELS) -I$(RT)

LIBEXE = libshapes.a

OBJ = shape.o sphere.o assembly.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

shape.o: shape.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)

sphere.o: sphere.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)

assembly.o: assembly.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)
