CC       = g++
FLAGS    = -O3 -Wall
LIBS     = -lm -lGL -lGLU -lglut -lsociety -lgraphics
LIBPATHS = -L./kernel/ -L./graphics/
INC      = -I./kernel/ -I./graphics/
OBJDIR   = objdir

OBJS = $(OBJDIR)/main.o   \
       $(OBJDIR)/facade.o \
#

EXE = whee

.NOTPARALLEL: all

all: directories $(OBJS)
	cd kernel   && $(MAKE)
	cd graphics && $(MAKE)
	$(CC) $(OBJS) $(FLAGS) $(LIBPATHS) $(LIBS) -o $(EXE)

directories:
	mkdir -p $(OBJDIR)

clean:
	make -C kernel   clean
	make -C graphics clean
	rm $(OBJS) $(EXE)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(INC) $(FLAGS) $^ -c -o $@
