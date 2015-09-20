CPPC = g++
CL = ar rcs

OBJ = fileio.o

LIBEXE = libutils.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

fileio.o: fileio.cpp
	$(CPPC) $^ -c
