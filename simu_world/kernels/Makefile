BASE  = ../..

CPPC  = g++
CL    = ar rcs
FLAGS = -O3 -Wall -lGL -lGLU -lglut
INC   = -I$(BASE)/kernels/

OBJ = map.o user.o       \
      draw_scene.o       \
      point_conversion.o \
      draw_block.o       \
      hud.o              \
      change_direction.o \
#

LIBEXE = libsimu.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

map.o: map.cpp
	$(CPPC) $^ -c

user.o: user.cpp
	$(CPPC) $^ $(INC) -c $(FLAGS)

draw_scene.o: draw_scene.cpp
	$(CPPC) $^ $(INC) -c $(FLAGS)

point_conversion.o: point_conversion.cpp
	$(CPPC) $^ $(INC) -c $(FLAGS)

draw_block.o: draw_block.cpp
	$(CPPC) $(INC) $^ -c

hud.o: hud.cpp
	$(CPPC) $(INC) $^ -c

change_direction.o: change_direction.cpp
	$(CPPC) $(INC) $^ -c
