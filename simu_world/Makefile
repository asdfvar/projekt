CPPC  = g++
FLAGS = -O3 -Wall -lm -lGL -lGLU -lglut
INC   = -Imap -I.
LIBPATH = -Lmap
LIBS  = -lsimu

OBJ = main.o simu_world.o opengl_interface.o \
      idle.o display.o time_manager.o

EXE = simu_world

all: SIMU_WORLD

SIMU_WORLD: $(OBJ)
	cd map/ && $(MAKE)
	$(CPPC) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE) $(FLAGS)

clean:
	make -C map/ clean
	rm $(OBJ) $(EXE)

map.o: map.cpp
	$(CPPC) $(INC) $^ -c

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
