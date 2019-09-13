#include "society.h"

Society::Society (void)
{
   map = new float[40 * 40 * 40];
}

Society::~Society (void)
{
   delete[] map;
}
