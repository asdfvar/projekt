CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

BASE = ..
KERNELS = $(BASE)/kernels
SHAPES = $(BASE)/objects
UTILITIES = $(BASE)/utilities

INC = -I. -I$(KERNELS) -I$(SHAPES) -I$(UTILITIES)

LIBEXE = libraytrace.a

OBJ = ray_trace.o ray.o run_ray_trace.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

ray_trace.o: ray_trace.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)

ray.o: ray.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)

run_ray_trace.o: run_ray_trace.cpp
	$(CPPC) $^ -c $(INC) $(FLAGS)
