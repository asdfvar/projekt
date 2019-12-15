CC = g++

OPTS = -O3

INC = -I../../

OBJS = test1.o               \
       test2.o               \
       test3.o               \
       test_2D.o             \
       test_cost_function2.o \
       timer.o               \
       pathfinding.o         \
#

all: TEST1 TEST2 TEST3 TEST_2D TEST_COST_FUNCTION2

TEST1: $(OBJS)
	$(CC) $(OPTS) test1.o pathfinding.o timer.o -o test1

TEST2: $(OBJS)
	$(CC) $(OPTS) test2.o pathfinding.o timer.o -o test2

TEST3: $(OBJS)
	$(CC) $(OPTS) test3.o pathfinding.o timer.o -o test3

TEST_2D: $(OBJS)
	$(CC) $(OPTS) test_2D.o pathfinding.o timer.o -o test_2D

TEST_COST_FUNCTION2: $(OBJS)
	$(CC) $(OPTS) test_cost_function2.o pathfinding.o timer.o -o test_cost_function2

clean:
	rm $(OBJS) test1 test2 test3 test_2D test_cost_function2

test1.o: test1.cpp
	$(CC) $(OPTS) $(INC) -c $^

test2.o: test2.cpp
	$(CC) $(OPTS) $(INC) -c $^

test3.o: test3.cpp
	$(CC) $(OPTS) $(INC) -c $^

test_2D.o: test_2D.cpp
	$(CC) $(OPTS) $(INC) -c $^

test_cost_function2.o: test_cost_function2.cpp
	$(CC) $(OPTS) $(INC) -c $^

pathfinding.o: ../../pathfinding.cpp
	$(CC) $(OPTS) $(INC) -c $^

timer.o: timer.cpp
	$(CC) $(OPTS) $(INC) -c $^
