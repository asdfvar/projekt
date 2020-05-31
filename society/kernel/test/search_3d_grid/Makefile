CC = g++

OBJS = test.o        \
       timer.o       \
       pathfinding.o \
#

all: TEST

TEST: $(OBJS)
	$(CC) $(OPTS) $^ -o test

clean:
	rm $(OBJS) test

%.o: %.cpp
	$(CC) $(OPTS) $(INC) -c $^
