BASE  = ../..

CPPC  = g++
CL    = ar rcs
FLAGS = -O3 -Wall -lGL -lGLU -lglut
INC   = -I$(BASE)/kernels/

OBJ = map.o              \
      chunk.o            \
      user.o             \
      point_conversion.o \
      draw_block.o       \
      hud.o              \
      change_direction.o \
      opengl_interface.o \
      fileio.o           \
      text.o             \
#

LIBEXE = libsimu.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

map.o: map.cpp
	$(CPPC) $^ -c

chunk.o: chunk.cpp
	$(CPPC) $^ -c

user.o: user.cpp
	$(CPPC) $^ $(INC) -c $(FLAGS)

point_conversion.o: point_conversion.cpp
	$(CPPC) $^ $(INC) -c $(FLAGS)

draw_block.o: draw_block.cpp
	$(CPPC) $(INC) $^ -c

hud.o: hud.cpp
	$(CPPC) $(INC) $^ -c

change_direction.o: change_direction.cpp
	$(CPPC) $(INC) $^ -c

opengl_interface.o: opengl_interface.cpp
	$(CPPC) $(INC) $^ -c

fileio.o: fileio.cpp
	$(CPPC) $(INC) $^ -c

text.o: text.cpp
	$(CPPC) $(INC) $^ -c
