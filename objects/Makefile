CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall -g

BASE = ..
KERNELS = $(BASE)/kernels
RT = $(BASE)/RT

INC = -I$(KERNELS) -I$(RT)

LIBEXE = libshapes.a

OBJ = shape.o sphere.o plane.o assembly.o light_source.o

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

light_source.o: light_source.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)

plane.o: plane.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)
