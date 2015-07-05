CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

LIBEXE = libkernels.a

OBJ = point_to_sphere.o intersect_sphere.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

point_to_sphere.o: point_to_sphere.cpp
	$(CPPC) $^ -c $(FLAGS)

intersect_sphere.o: intersect_sphere.cpp
	$(CPPC) $^ -c $(FLAGS)
