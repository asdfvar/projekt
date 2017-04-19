BASE  = ../

CPPC  = g++
CL    = ar rcs
FLAGS = -O3 -Wall -lGL -lGLU -lglut
INC   = -I$(BASE)/kernels/ \

OBJ = map.o          \
      chunk.o        \
      chunks.o       \
      access_chunk.o \
      shift.o        \
      render_chunk.o \
      update.o       \
      queue.o        \
      read_chunk.o   \
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

chunks.o: chunks.cpp
	$(CPPC) $(INC) $^ -c

access_chunk.o: access_chunk.cpp
	$(CPPC) $(INC) $^ -c

shift.o: shift.cpp
	$(CPPC) $(INC) $^ -c

render_chunk.o: render_chunk.cpp
	$(CPPC) $(INC) $^ -c

update.o: update.cpp
	$(CPPC) $(INC) $^ -c

queue.o: queue.cpp
	$(CPPC) $(INC) $^ -c

read_chunk.o: read_chunk.cpp
	$(CPPC) $(INC) $^ -c
