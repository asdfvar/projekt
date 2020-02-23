#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "math_utils.h"

int main (void)
{

   float *array  = new float[80000];
   float *buffer = new float[80000];

   int    size[2] = {100, 100};
   float  scale = 4.0f;
   int    num_grid_cells[2] = {6, 6};
   float  min_grid_point_dist = 8.0f;

   for (int k = 0; k < size[0]*size[1]; k++) array[k] = 0.0f;

   perlin (
         array,
         size,
         scale,
         num_grid_cells,
         min_grid_point_dist,
         buffer);

   num_grid_cells[0] = 10;
   num_grid_cells[1] = 10;
   scale = 1.0f;

   perlin (
         array,
         size,
         scale,
         num_grid_cells,
         min_grid_point_dist,
         buffer);

#if 1
   std::ofstream output;
   output.open ("output", std::ios::out);

   for (int row = 0, ind = 0; row < size[1]; row++)
   {
      for (int col = 0; col < size[0]; col++, ind++)
      {
         output << std::setprecision(3) << array[ind];
         if (col == size[0] - 1) output << "; ";
         else output << ", ";
      }

      output << std::endl;
   }

   output.close ();
#endif

   delete[] array;
   delete[] buffer;

   return 0;
}
