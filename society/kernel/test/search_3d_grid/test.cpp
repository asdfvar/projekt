#include "pathfinding.h"
#include "timer.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

#define DIM_X 8
#define DIM_Y 8
#define DIM_Z 4

int main ()
{
   bool *nodes = new bool [DIM_X * DIM_Y * DIM_Z];

   for (int ind = 0; ind < DIM_X * DIM_Y * DIM_Z; ind++) nodes[ind] = false;

   int src[3] = { 1,     2,     1     };
   int dim[3] = { DIM_X, DIM_Y, DIM_Z };

   int open_cell[3] = { 3, 4, 2 };
   int flat_ind = dim_to_flat_ind (3, open_cell, dim);

   nodes[flat_ind] = true;

   std::cout << "open cell at ("
      << open_cell[0] << ", "
      << open_cell[1] << ", "
      << open_cell[1] << ") has flat index = "
      << flat_ind << " and the found location = ";

   int location = search (nodes, dim, src);

   std::cout << location << std::endl;

   delete[] nodes;

   return 0;
}
