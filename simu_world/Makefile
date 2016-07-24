CPPC  = g++
FLAGS = -O3 -Wall -lm -lGL -lGLU -lglut

OBJ = main.o simu_world.o opengl_interface.o \
      idle.o display.o time_manager.o

EXE = simu_world

all: $(OBJ)
	$(CPPC) $^ -o $(EXE) $(FLAGS)

clean:
	rm $(OBJ) $(EXE)

map.o: map.cpp
	$(CPPC) $^ -c

opengl_interface.o: opengl_interface.cpp
	$(CPPC) $^ -c

simu_world.o: simu_world.cpp
	$(CPPC) $^ -c

idle.o: idle.cpp
	$(CPPC) $^ -c

display.o: display.cpp
	$(CPPC) $^ -c

time_manager.o: time_manager.cpp
	$(CPPC) $^ -c
