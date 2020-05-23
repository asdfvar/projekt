CC       = g++
FLAGS    = -Wall -g -Wno-unused-function
LIBS     = -lm -pthread -lGL -lGLU -lglut -lsociety -lgraphics -lmenu_system
LIBPATHS = -L./kernel/ -L./graphics/ -L./menu_system/
INC      = -I./kernel/ -I./graphics/ -I./menu_system/
OBJDIR   = objdir

OBJS = $(OBJDIR)/main.o        \
       $(OBJDIR)/facade.o      \
#

EXE = whee

.NOTPARALLEL: all

all: directories $(OBJS)
	cd kernel   &&    $(MAKE) -f society.mk
	cd graphics &&    $(MAKE) -f graphics.mk
	cd menu_system && $(MAKE) -f menu_system.mk
	$(CC) $(OBJS) $(FLAGS) $(LIBPATHS) $(LIBS) -o $(EXE)

directories:
	mkdir -p $(OBJDIR)

clean:
	$(MAKE) -C kernel      -f society.mk     clean
	$(MAKE) -C graphics    -f graphics.mk    clean
	$(MAKE) -C menu_system -f menu_system.mk clean
	rm $(OBJS) $(EXE)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(INC) $(FLAGS) $^ -c -o $@
