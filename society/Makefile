CC     = g++
FLAGS  = -O3 -Wall
LIBS   = -lm -lGL -lGLU -lglut
INC    = -I./kernel/
OBJDIR = objdir

OBJS = $(OBJDIR)/main.o   \
       $(OBJDIR)/facade.o \
#

EXE = whee

.NOTPARALLEL: all

all: directories $(OBJS)
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o $(EXE)

directories:
	mkdir -p $(OBJDIR)

clean:
	rm $(OBJS) $(EXE)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(INC) $(FLAGS) $^ -c -o $@
