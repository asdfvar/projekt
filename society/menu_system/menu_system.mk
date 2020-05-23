BASE_DIR = ../

include $(BASE_DIR)/directives.mk

CC = g++

OBJDIR = objdir

INC = -I$(BASE_DIR)/graphics/

OBJS = $(OBJDIR)/menu_system.o         \
       $(OBJDIR)/main_menu.o           \
       $(OBJDIR)/build_menu.o          \
       $(OBJDIR)/buildMaterial_menu.o  \
#

LIBNAME = libmenu_system.a

all: directories $(OBJS)
	ar rcs $(LIBNAME) $(OBJS)

directories:
	@mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR) $(LIBNAME)
	@echo "cleaned menu system library"

$(OBJDIR)/%.o: %.cpp
	$(CC) $^ $(DIRECTIVES) $(INC) -c -o $@
