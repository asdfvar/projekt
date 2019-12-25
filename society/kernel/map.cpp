#include "map.h"
#include <cstdlib>

MAP::MAP (int num_cells[3])
{
   size[0] = num_cells[0];
   size[1] = num_cells[1];
   size[2] = num_cells[2];

   map    = new float[size[0] * size[1] * size[2]];
   ground = new float[size[0] * size[1] * size[2]];

   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++) map[ind] = 1.0f;

   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            if (rand() & 3 && ind_z == 20) map[ind] = 1.0f;
            else if (ind_z == 22) map[ind] = 1.0f;
            else if (ind_x == ind_z && ind_y == 0) map[ind] = 1.0f;
            else map[ind] = -1.0f;
         }
      }
   }

   set_ground ();
}

MAP::~MAP (void)
{
   delete[] map;
   delete[] ground;
}

void MAP::set_ground (void)
{
   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            // Set the ground value to the map value if there is ground below this cell
            if (ind_z == 0) ground[ind] = map[ind];
            else if (map[ind - size[0] * size[1]] <= 0) ground[ind] = map[ind];

            // Default is -1
            else ground[ind] = -1.0f;
         }
      }
   }
}
