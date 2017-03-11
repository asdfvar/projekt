BASE  = ../

CPPC  = g++
CL    = ar rcs
FLAGS = -O3 -Wall -lGL -lGLU -lglut
INC   = -I$(BASE)/kernels/ \

OBJ = map.o   \
      chunk.o \
#

LIBEXE = libmap.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

map.o: map.cpp
	$(CPPC) $(INC) $^ -c

chunk.o: chunk.cpp
	$(CPPC) $(INC) $^ -c
