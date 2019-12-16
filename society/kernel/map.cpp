#include "map.h"
#include <cstdlib>

MAP::MAP (int num_cells[3])
{
   dim_x = num_cells[0];
   dim_y = num_cells[1];
   dim_z = num_cells[2];

   map = new float[dim_x * dim_y * dim_z];
   for (int ind = 0; ind < dim_x * dim_y * dim_z; ind++) map[ind] = 1.0f;

   for (int ind_z = 0, ind = 0; ind_z < dim_z; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
            if (rand() & 3 && ind_z == 20) map[ind] = 1.0f;
            else map[ind] = -1.0f;
         }
      }
   }
}

MAP::~MAP (void)
{
   delete[] map;
}
