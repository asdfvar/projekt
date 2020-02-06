CC = g++

OBJS = test.o      \
       container.o \
#

EXE = test

all: $(OBJS)
	$(CC) -g $^ -o $(EXE)

clean:
	rm -v $(EXE) $(OBJS)

%.o: %.cpp
	$(CC) -g $^ -c
