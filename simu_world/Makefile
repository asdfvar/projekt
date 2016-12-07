CPPC  = g++
FLAGS = -O3 -Wall -lm -lGL -lGLU -lglut
INC   = -Ikernels -I.
LIBPATH = -Lkernels
LIBS  = -lsimu

OBJ = main.o             \
      simu_world.o       \
      opengl_interface.o \
      idle.o             \
      display.o          \
      time_manager.o     \
#

EXE = simu_world

all: SIMU_WORLD

SIMU_WORLD: $(OBJ)
	cd kernels/ && $(MAKE)
	$(CPPC) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE) $(FLAGS)

clean:
	make -C kernels/ clean
	rm $(OBJ) $(EXE)

opengl_interface.o: opengl_interface.cpp
	$(CPPC) $(INC) $^ -c

simu_world.o: simu_world.cpp
	$(CPPC) $(INC) $^ -c

idle.o: idle.cpp
	$(CPPC) $(INC) $^ -c

display.o: display.cpp
	$(CPPC) $(INC) $^ -c

time_manager.o: time_manager.cpp
	$(CPPC) $(INC) $^ -c
