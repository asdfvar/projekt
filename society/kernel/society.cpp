#include "map.h"
#include "society.h"
#include "pathfinding.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

Society::Society (void)
{
   dim_x = 40;
   dim_y = 40;
   dim_z = 40;

   map_layer = 20;

   int dim[3] = {dim_x, dim_y, dim_z};

   Map = new MAP (dim, map_layer);

   bool found = false;

   float unit_x = 0.0f;
   float unit_y = 0.0f;
   float unit_z = 0.0f;

   for (int ind_z = 0, ind = 0; ind_z < dim_z && !found; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y && !found; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x && !found; ind_x++, ind++)
         {
            const float *map = Map->access_map ();
            if (map[ind] > 0.0f && ind_z == map_layer) {
               found = true;
               unit_x = (float)ind_x     + 0.5f;
               unit_y = (float)ind_y     + 0.5f;
               unit_z = (float)map_layer + 0.5f;
            }
         }
      }
   }

   float *scratch = new float[2 * dim_x * dim_y * dim_z];
   Unit *unit = new Unit (unit_x, unit_y, unit_z, Map, scratch);

   units.push_back (unit);
}

Society::~Society (void)
{
   delete Map;
}

void Society::update (float time_step)
{
   units[0]->update (time_step);
}

const float *Society::access_map (int *dim_x_out, int *dim_y_out, int *dim_z_out)
{
   *dim_x_out = dim_x;
   *dim_y_out = dim_y;
   *dim_z_out = dim_z;
   return static_cast<const float*>(Map->access_map ());
}

int Society::get_unit_positions (float *x, float *y, float *z)
{
   int ind = 0;
   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++, ind++) {
      x[ind] = (*it)->get_position_x();
      y[ind] = (*it)->get_position_y();
      z[ind] = (*it)->get_position_z();
   }

   return ind;
}
