CC = g++

OPTS = -O3

INC = -I../

OBJS = test1.o       \
       test2.o       \
       timer.o       \
       pathfinding.o \
#

all: TEST1 TEST2

TEST1: $(OBJS)
	$(CC) $(OPTS) test1.o pathfinding.o timer.o -o test1

TEST2: $(OBJS)
	$(CC) $(OPTS) test2.o pathfinding.o timer.o -o test2

clean:
	rm $(OBJS) test1 test2

test1.o: test1.cpp
	$(CC) $(OPTS) $(INC) -c $^

test2.o: test2.cpp
	$(CC) $(OPTS) $(INC) -c $^

pathfinding.o: ../pathfinding.cpp
	$(CC) $(OPTS) $(INC) -c $^

timer.o: timer.cpp
	$(CC) $(OPTS) $(INC) -c $^
