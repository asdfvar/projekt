BASE  = ../

CPPC            = g++
CL              = ar rcs
FLAGS           = -O3 -Wall
DIRECTIVE_FLAGS = -DBLOCKS
INC             = -I$(BASE)          \
                  -I$(BASE)/kernels/ \
#

OBJ = map.o          \
      shift.o        \
      render_chunk.o \
      update.o       \
      queue.o        \
      read_chunk.o   \
#

DEBUG ?= FALSE
ifeq ($(DEBUG), TRUE)
   DEFINES = -DDEBUG
else
   DEFINES =
endif
DEFINES = -DDEBUG

LIBEXE = libmap.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

map.o: map.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

shift.o: shift.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

render_chunk.o: render_chunk.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

update.o: update.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

queue.o: queue.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)

read_chunk.o: read_chunk.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c $(DIRECTIVE_FLAGS) $(FLAGS)
