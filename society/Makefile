CC       = g++
FLAGS    = -O3 -Wall -g
LIBS     = -lm -pthread -lGL -lGLU -lglut -lsociety -lgraphics
LIBPATHS = -L./kernel/ -L./graphics/
INC      = -I./kernel/ -I./graphics/
OBJDIR   = objdir

OBJS = $(OBJDIR)/main.o        \
       $(OBJDIR)/facade.o      \
       $(OBJDIR)/menu_system.o \
#

EXE = whee

.NOTPARALLEL: all

all: directories $(OBJS)
	cd kernel   && $(MAKE) -f society.mk
	cd graphics && $(MAKE) -f graphics.mk
	$(CC) $(OBJS) $(FLAGS) $(LIBPATHS) $(LIBS) -o $(EXE)

directories:
	mkdir -p $(OBJDIR)

clean:
	$(MAKE) -C kernel -f society.mk clean
	$(MAKE) -C graphics -f graphics.mk clean
	rm $(OBJS) $(EXE)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(INC) $(FLAGS) $^ -c -o $@
