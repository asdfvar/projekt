CC       = g++
FLAGS    = -O3 -Wall
LIBS     = -lm -lGL -lGLU -lglut -lsociety
LIBPATHS = -L./kernel/
INC      = -I./kernel/
OBJDIR   = objdir

OBJS = $(OBJDIR)/main.o   \
       $(OBJDIR)/facade.o \
#

EXE = whee

.NOTPARALLEL: all

all: directories $(OBJS)
	cd kernel && $(MAKE)
	$(CC) $(OBJS) $(FLAGS) $(LIBPATHS) $(LIBS) -o $(EXE)

directories:
	mkdir -p $(OBJDIR)

clean:
	make -C kernel clean
	rm $(OBJS) $(EXE)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(INC) $(FLAGS) $^ -c -o $@
