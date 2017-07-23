BASE  = ../

CPPC  = g++
CL    = ar rcs
FLAGS = -O3 -Wall
DIRECTIVE_FLAGS = -DBLOCKS
INC   = -I$(BASE)/kernels/ \
#

OBJ = map.o          \
      chunk.o        \
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
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

chunk.o: chunk.cpp
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

shift.o: shift.cpp
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

render_chunk.o: render_chunk.cpp
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

update.o: update.cpp
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

queue.o: queue.cpp
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

read_chunk.o: read_chunk.cpp
	$(CPPC) $(INC) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)
