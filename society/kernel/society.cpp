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

   int dim[3] = {dim_x, dim_y, dim_z};

   Map = new MAP (dim);

   float dest[3];

   int map_layer = 20;

   int num_units = 16;

   int unit_count = 0;

   for (int ind_z = 0, ind = 0; ind_z < dim_z; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
            const float *map = Map->access_map ();
            if (map[ind] > 0.0f && ind_z == map_layer && unit_count < num_units) {
               unit_count++;
               dest[0] = (float)ind_x     + 0.5f;
               dest[1] = (float)ind_y     + 0.5f;
               dest[2] = (float)map_layer + 0.5f;

               units.push_back (new Unit (dest[0], dest[1], dest[2], Map));
            }
         }
      }
   }

}

Society::~Society (void)
{
   delete Map;
}

void Society::set_destination (int destination[3])
{
   const float *map = Map->access_map ();

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++) {
      (*it)->set_destination (destination);
   }
}

void Society::update (float time_step)
{
   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++) {
      (*it)->update (units, time_step);
   }
}

const float *Society::access_map (int *dim_x_out, int *dim_y_out, int *dim_z_out)
{
   *dim_x_out = dim_x;
   *dim_y_out = dim_y;
   *dim_z_out = dim_z;
   return static_cast<const float*>(Map->access_map ());
}

int Society::get_unit_info (float *x, float *y, float *z, bool *selected)
{
   int ind = 0;
   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++, ind++) {
      x[ind] = (*it)->get_position_x();
      y[ind] = (*it)->get_position_y();
      z[ind] = (*it)->get_position_z();
      selected[ind] = (*it)->get_selected();
   }

   return ind;
}
void Society::select_units (float *selection_box, int map_layer, bool control_down)
{
   if (control_down == false)
   {
      for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++)
      {
         (*it)->unselect();
      }
   }

   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++)
   {
      float x = (*it)->get_position_x();
      float y = (*it)->get_position_y();
      float z = (*it)->get_position_z();

      if (z < (float)map_layer || z > (float)(map_layer + 1)) continue;

      float min_x = (selection_box[0] + 1.0f) / 2.0f * (float)dim_x;
      float max_x = (selection_box[2] + 1.0f) / 2.0f * (float)dim_x;

      if (max_x < min_x) {
         float temp = min_x;
         min_x = max_x;
         max_x = temp;
      }

      float min_y = (selection_box[1] + 1.0f) / 2.0f * (float)dim_y;
      float max_y = (selection_box[3] + 1.0f) / 2.0f * (float)dim_y;

      if (max_y < min_y) {
         float temp = min_y;
         min_y = max_y;
         max_y = temp;
      }

      if (x >= min_x && x <= max_x &&
          y >= min_y && y <= max_y)
      {
         (*it)->select();
      }
   }
}

void Society::select_all (void)
{
   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++)
      (*it)->select();
}

void Society::unselect_all (void)
{
   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++)
      (*it)->unselect();
}
