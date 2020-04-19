BASE_DIR = ../

include $(BASE_DIR)/directives.mk

CC = g++

INC = -I$(BASE_DIR)

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
	$(CC) $^ $(INC) $(DIRECTIVES) -c -o $@
