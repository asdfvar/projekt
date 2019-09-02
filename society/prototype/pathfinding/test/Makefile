CC = g++

INC = -I../

OBJS = test.o        \
       pathfinding.o \
#

all: $(OBJS)
	$(CC) $^ -o test

clean:
	rm $(OBJS) test

test.o: test.cpp
	$(CC) $(INC) -c $^

pathfinding.o: ../pathfinding.cpp
	$(CC) $(INC) -c $^
