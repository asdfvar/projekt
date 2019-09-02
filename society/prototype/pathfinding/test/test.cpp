#include "pathfinding.h"
#include <iostream>
#include <iomanip>

#define DIM_X 5
#define DIM_Y 5
#define DIM_Z 5

int main ()
{

   float *nodes  = new float[DIM_X * DIM_Y * DIM_Z];
   float *cost   = new float[DIM_X * DIM_Y * DIM_Z];
   float *buffer = new float[DIM_X * DIM_Y * DIM_Z];

   int dim[3] = { DIM_X, DIM_Y, DIM_Z };

   int src[3] = { 3, 3, 0 };
   int dst[3] = { 6, 6, 0 };

   dst[0] = -1;
//   nodes[DIM_X * 3 + 5] = -1;

   for (int k = 0, ind = 0; k < DIM_Z; k++)
      for (int j = 0; j < DIM_Z; j++)
         for (int i = 0; i < DIM_Z; i++, ind++)
            nodes[ind] = 0;

   cost_function (nodes, cost, dim, src, dst, buffer);

   for (int k = 0, ind = 0; k < DIM_Z; k++)
   {
      std::cout << std::endl;
      for (int j = 0; j < DIM_Y; j++)
      {
         std::cout << std::endl;
         for (int i = 0; i < DIM_X; i++, ind++)
         {
            std::cout << std::setw(10) << cost[ind];
         }
      }
   }
   std::cout << std::endl;

   delete[] nodes;
   delete[] cost;
   delete[] buffer;

   return 0;
}
