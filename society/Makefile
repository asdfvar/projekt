CC = g++
FLAGS = -O3 -Wall
LIBS = -lm -lGL -lGLU -lglut

OBJS = main.o   \
       facade.o \
#

EXE = whee

all: $(OBJS)
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o $(EXE)

clean:
	rm $(OBJS) $(EXE)

%.o: %.cpp
	$(CC) $(FLAGS) $^ -c
