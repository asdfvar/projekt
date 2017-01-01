CPPC  = g++
FLAGS = -O3 -Wall -lm -lGL -lGLU -lglut
INC   = -Ikernels -I.
LIBPATH = -Lkernels
LIBS  = -lsimu

OBJ = main.o             \
      simu_world.o       \
      simu_world_obj.o   \
      time_manager.o     \
      semaphore.o        \
      io_manager.o       \
#

EXE = simu_world

all: SIMU_WORLD

SIMU_WORLD: $(OBJ)
	cd kernels/ && $(MAKE)
	$(CPPC) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE) $(FLAGS)

clean:
	make -C kernels/ clean
	rm $(OBJ) $(EXE)

simu_world_obj.o: simu_world_obj.cpp
	$(CPPC) $(INC) $^ -c

simu_world.o: simu_world.cpp
	$(CPPC) $(INC) $^ -c

time_manager.o: time_manager.cpp
	$(CPPC) $(INC) $^ -c

semaphore.o: semaphore.cpp
	$(CPPC) $(INC) $^ -c

io_manager.o: io_manager.cpp
	$(CPPC) $(INC) $^ -c
