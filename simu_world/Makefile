CPPC  = g++
FLAGS = -O3 -Wall -lm -lGL -lGLU -lglut
INC   = -Ikernels -I.
LIBPATH = -Lkernels
LIBS  = -lsimu -lpthread

DEBUG ?= FALSE
ifeq ($(DEBUG), TRUE)
   DEFINES = -DDEBUG
else
   DEFINES =
endif

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
	$(CPPC) $^ $(INC) $(DEFINES) $(LIBS) $(LIBPATH) -o $(EXE) $(FLAGS)

clean:
	make -C kernels/ clean
	rm $(OBJ) $(EXE)

simu_world_obj.o: simu_world_obj.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

simu_world.o: simu_world.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

time_manager.o: time_manager.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

semaphore.o: semaphore.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

io_manager.o: io_manager.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c
