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
   float  scale = 1.0f;
   int    num_grid_cells[2] = {6, 6};
   float  min_grid_point_dist = 8.0f;

   float *matrix = array;

   for (int ind = 0; ind < 4 * 5; ind++)
      matrix[ind] = (float)(rand () % 100) / 100.0f;

#if 0
   std::cout << "matrix before RREF:" << std::endl;
   for (int row = 0, ind = 0; row < 4; row++)
   {
      for (int col = 0; col < 5; col++, ind++)
         std::cout << matrix[ind] << ", ";
      std::cout << std::endl;
   }
   std::cout << std::endl;
#endif

   rref (matrix, 4, 5);

#if 0
   std::cout << "matrix after RREF:" << std::endl;
   for (int row = 0, ind = 0; row < 4; row++)
   {
      for (int col = 0; col < 5; col++, ind++)
         std::cout << matrix[ind] << ", ";
      std::cout << std::endl;
   }
   std::cout << std::endl;
#endif

   perlin (
         array,
         size,
         scale,
         num_grid_cells,
         min_grid_point_dist,
         buffer);

#if 1
   std::ofstream asdf;
   asdf.open ("output", std::ios::out);
   for (int row = 0, ind = 0; row < size[1]; row++)
   {
      for (int col = 0; col < size[0]; col++, ind++)
      {
         asdf << std::setprecision(3) << array[ind];
         if (col == size[0] - 1) asdf << "; ";
         else asdf << ", ";
      }
      asdf << std::endl;
   }
asdf << "stuff\n";
   asdf.close ();
#endif

   delete[] array;
   delete[] buffer;

   return 0;
}
