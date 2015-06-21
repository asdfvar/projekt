CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

LIBEXE = libkernels.a

OBJ = linalg.o point_to_sphere.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

linalg.o: linalg.cpp
	$(CPPC) $^ -c $(FLAGS)

point_to_sphere.o: point_to_sphere.cpp
	$(CPPC) $^ -c $(FLAGS)
