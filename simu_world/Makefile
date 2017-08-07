CPPC    = g++
FLAGS   = -O3 -Wall -lm -lpthread -lGL -lGLU -lglut
INC     = -Ikernels -Imap -I.
LIBPATH = -Lkernels -Lmap
LIBS    = -lmap -lsimu

DEBUG ?= TRUE
ifeq ($(DEBUG), TRUE)
   DEFINES = -DDEBUG
else
   DEFINES =
endif

OBJ = main.o             \
      simu_world.o       \
      simu_world_obj.o   \
      semaphore.o        \
      io_manager.o       \
#

EXE = simu_world

all: SIMU_WORLD

SIMU_WORLD: $(OBJ)
	cd kernels/ && $(MAKE)
	cd map/ &&     $(MAKE)
	$(CPPC) $^ $(INC) $(DEFINES) $(LIBS) $(LIBPATH) -o $(EXE) $(FLAGS)

clean:
	make -C kernels/ clean
	make -C map/     clean
	rm $(OBJ) $(EXE)

main.o: main.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

simu_world_obj.o: simu_world_obj.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

simu_world.o: simu_world.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

semaphore.o: semaphore.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c

io_manager.o: io_manager.cpp
	$(CPPC) $(INC) $(DEFINES) $^ -c
