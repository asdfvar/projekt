CC = g++

OBJDIR = objdir

OBJS = $(OBJDIR)/graphics.o \
#

LIBNAME = libgraphics.a

all: objdir $(OBJS)
	ar rcs $(LIBNAME) $(OBJS)

clean:
	rm $(OBJS) $(LIBNAME)

objdir:
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	$(CC) $^ -c -o $@
