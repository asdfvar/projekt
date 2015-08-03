CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

BASE = ..
KERNELS = $(BASE)/kernels

INC = -I$(KERNELS)

LIBEXE = libraytrace.a

OBJ = ray.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

ray.o: ray.cpp
	$(CPPC) $^ -c $(FLAGS)
