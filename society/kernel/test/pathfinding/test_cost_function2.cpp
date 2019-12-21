#include "pathfinding.h"
#include <iostream>
#include <iomanip>

#define DIM_X 8
#define DIM_Y 5
#define DIM_Z 5

int main ()
{

   float *nodes        = new float[DIM_X * DIM_Y * DIM_Z];
   float *cost         = new float[DIM_X * DIM_Y * DIM_Z];
   float *buffer       = new float[DIM_X * DIM_Y * DIM_Z];
   int   *cost_indices = new int[DIM_X * DIM_Y * DIM_Z];

   int dim[3] = { DIM_X, DIM_Y, DIM_Z };

   int start[3] = { 3, 2, 0 };

   for (int ind = 0; ind < DIM_X * DIM_Y * DIM_Z; ind++) nodes[ind] = 0.0f;

#if 0
   nodes[0 * DIM_X * DIM_Y + 1 * DIM_X + 5] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 2 * DIM_X + 5] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 3 * DIM_X + 5] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 3 * DIM_X + 4] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 3 * DIM_X + 3] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 3 * DIM_X + 2] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 1 * DIM_X + 0] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 1 * DIM_X + 1] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 1 * DIM_X + 2] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 1 * DIM_X + 3] = -1.0f;
   nodes[0 * DIM_X * DIM_Y + 1 * DIM_X + 4] = -1.0f;

   nodes[1 * DIM_X * DIM_Y + 1 * DIM_X + 0] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 1 * DIM_X + 1] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 1 * DIM_X + 2] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 1 * DIM_X + 3] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 1 * DIM_X + 4] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 1 * DIM_X + 5] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 2 * DIM_X + 2] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 2 * DIM_X + 3] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 2 * DIM_X + 4] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 2 * DIM_X + 5] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 3 * DIM_X + 2] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 3 * DIM_X + 3] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 3 * DIM_X + 4] = -1.0f;
   nodes[1 * DIM_X * DIM_Y + 3 * DIM_X + 5] = -1.0f;

   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 0] = -1.0f;
   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 1] = -1.0f;
   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 2] = -1.0f;
   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 3] = -1.0f;
   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 4] = -1.0f;
   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 5] = -1.0f;
   nodes[2 * DIM_X * DIM_Y + 1 * DIM_X + 6] = -1.0f;

   for (int ind = 1; ind < DIM_X * DIM_Y; ind++)
      nodes[3 * DIM_X * DIM_Y + ind] = -1.0f;

   nodes[4 * DIM_X * DIM_Y + 3 * DIM_X + 1] = -1.0f;
   nodes[4 * DIM_X * DIM_Y + 3 * DIM_X + 2] = -1.0f;
   nodes[4 * DIM_X * DIM_Y + 3 * DIM_X + 3] = -1.0f;
   nodes[4 * DIM_X * DIM_Y + 0 * DIM_X + 4] = -1.0f;
   nodes[4 * DIM_X * DIM_Y + 1 * DIM_X + 4] = -1.0f;
   nodes[4 * DIM_X * DIM_Y + 2 * DIM_X + 4] = -1.0f;

   const int num_cells = 10;
#else
   const int num_cells = 40;
#endif

   // start from the destination point and branch outward
   cost_function2 (
         nodes,
         cost,
         cost_indices,
         dim,
         start,
         num_cells,
         buffer);

   for (int k = 0, ind = 0; k < DIM_Z; k++) {
      for (int j = 0; j < DIM_Y; j++) {
         for (int i = 0; i < DIM_X; i++, ind++)
         {
            if (cost[ind] < 9999.9f)
               std::cout << std::setw(12) << cost[ind];
            else
               std::cout << " -----------";
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }

   std::cout << "cost_indices = ";
   for (int ind = 0; ind < num_cells; ind++) std::cout << cost_indices[ind] << ", ";
   std::cout << std::endl;

   delete[] nodes;
   delete[] cost;
   delete[] cost_indices;
   delete[] buffer;

   return 0;
}
