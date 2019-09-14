CC = g++

OBJDIR = objdir

OBJS = $(OBJDIR)/society.o     \
       $(OBJDIR)/pathfinding.o \
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
