#include "pathfinding.h"
#include <iostream>
#include <iomanip>

#define DIM_X 18
#define DIM_Y 10

int main ()
{

   float *nodes  = new float[DIM_X * DIM_Y];
   float *cost   = new float[DIM_X * DIM_Y];
   int   *path   = new int  [DIM_X * DIM_Y];
   float *buffer = new float[DIM_X * DIM_Y];

   int dim[3] = { DIM_X, DIM_Y, 1 };

   int src[3] = {  0, 0, 0 };
   int dst[3] = { 17, 9, 0 };

   for (int ind = 0; ind < DIM_X * DIM_Y; ind++) nodes[ind] = 0.0f;

   nodes[0 * DIM_X + 5] = -1.0f;
   nodes[1 * DIM_X + 5] = -1.0f;

   // start from the destination point and branch outward
   cost_function (
         nodes,
         cost,
         dim,
         dst,       // source (cost = 0) will be the end point
         src,       // destination point will be the starting point
         buffer);

#if 1
   std::cout << std::endl;
   for (int j = 0, ind = 0; j < DIM_Y; j++)
   {
      std::cout << std::endl;
      for (int i = 0; i < DIM_X; i++, ind++)
      {
         std::cout << std::setw(12) << cost[ind];
      }
   }
   std::cout << std::endl;
#endif

   int path_size = pathfinding (
         cost,
         dim,
         src,
         dst,
         path);

   std::cout << "path = ";
   for (int ind = 0; ind < path_size; ind++) std::cout << path[ind] << ", ";
   std::cout << std::endl;

   for (int j = 0, ind = 0; j < DIM_Y; j++) {
      for (int i = 0; i < DIM_X; i++, ind++)
      {
         bool found = false;
         for (int search_ind = 0; search_ind < path_size; search_ind++)
            if (path[search_ind] == ind) found = true;

         if (ind == (src[2] * DIM_X * DIM_Y) + (src[1] * DIM_X) + src[0]) std::cout << "S";
         else if (ind == (dst[2] * DIM_X * DIM_Y)+(dst[1] * DIM_X) + dst[0]) std::cout << "E";
         else if (found)          std::cout << "x";
         else if (nodes[ind] < 0) std::cout << "O";
         else                     std::cout << ".";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;

   delete[] nodes;
   delete[] cost;
   delete[] path;
   delete[] buffer;

   return 0;
}
