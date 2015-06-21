CPPC = g++
CL = ar rcs
FLAGS = -O3 -Wall

LIBEXE = libshapes.a

OBJ = shape.o sphere.o assembly.o ray.o

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

shape.o: shape.cpp
	$(CPPC) $^ -c $(FLAGS)

sphere.o: sphere.cpp
	$(CPPC) $^ -c $(FLAGS)

assembly.o: assembly.cpp
	$(CPPC) $^ -c $(FLAGS)

ray.o: ray.cpp
	$(CPPC) $^ -c $(FLAGS)
