CPPC  = g++
CL    = ar rcs
FLAGS = -O3 -Wall

OBJ = map.o user.o

LIBEXE = libsimu.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

map.o: map.cpp
	$(CPPC) $^ -c

user.o: user.cpp
	$(CPPC) $^ -c