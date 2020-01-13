CC = g++

OBJDIR = objdir

OBJS = $(OBJDIR)/society.o     \
       $(OBJDIR)/pathfinding.o \
       $(OBJDIR)/unit.o        \
       $(OBJDIR)/map.o         \
       $(OBJDIR)/graphics.o    \
       $(OBJDIR)/actions.o     \
       $(OBJDIR)/timer.o       \
#

LIBNAME = libsociety.a

all: directories $(OBJS)
	ar rcs $(LIBNAME) $(OBJS)

directories:
	@mkdir -p $(OBJDIR)

clean:
	rm $(OBJS) $(LIBNAME)

$(OBJDIR)/%.o: %.cpp
	$(CC) $^ -c -o $@
