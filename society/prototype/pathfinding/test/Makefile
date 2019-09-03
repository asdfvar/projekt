CC = g++

INC = -I../

OBJS = test1.o       \
       test2.o       \
       timer.o       \
       pathfinding.o \
#

all: TEST1 TEST2

TEST1: $(OBJS)
	$(CC) test1.o pathfinding.o timer.o -o test1

TEST2: $(OBJS)
	$(CC) test2.o pathfinding.o timer.o -o test2

clean:
	rm $(OBJS) test1 test2

test1.o: test1.cpp
	$(CC) $(INC) -c $^

test2.o: test2.cpp
	$(CC) $(INC) -c $^

pathfinding.o: ../pathfinding.cpp
	$(CC) $(INC) -c $^

timer.o: timer.cpp
	$(CC) $(INC) -c $^
