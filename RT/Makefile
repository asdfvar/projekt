CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

BASE = ..
KERNELS = $(BASE)/kernels

INC = -I$(KERNELS)

LIBEXE = libraytrace.a

OBJ = ray_trace.o ray.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

ray.o: ray_trace.cpp
	$(CPPC) $^ -c $(FLAGS)

ray.o: ray.cpp
	$(CPPC) $^ -c $(FLAGS)
