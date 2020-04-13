BASE_DIR = ../

include $(BASE_DIR)/directives.mk

CC = g++

OBJDIR = objdir

OBJS = $(OBJDIR)/graphics.o \
       $(OBJDIR)/text.o     \
#

LIBNAME = libgraphics.a

all: objdir $(OBJS)
	ar rcs $(LIBNAME) $(OBJS)

clean:
	rm $(OBJS) $(LIBNAME)

objdir:
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	$(CC) $^ $(DIRECTIVES) -c -o $@
