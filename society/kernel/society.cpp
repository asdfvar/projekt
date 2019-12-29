#include "map.h"
#include "society.h"
#include "pathfinding.h"
#include "timer.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

Society::Society (void)
{
   dim_x = 60;
   dim_y = 60;
   dim_z = 40;

   int dim[3] = {dim_x, dim_y, dim_z};

   Map = new MAP (dim);

   float dest[3];

   int map_layer = 20;

   int num_units = 200;

   int unit_count = 0;

   accum_time       = 0.0f;
   accum_time_limit = 0.0f;
   rolled           = false;

   const float *map = Map->access_ground ();

   for (int ind_z = 0, ind = 0; ind_z < dim_z; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
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

   scratch = new int[4096];
   cost    = new float[dim_x * dim_y * dim_z];
   buffer  = new float[dim_x * dim_y * dim_z];
}

Society::~Society (void)
{
   delete   Map;
   delete[] scratch;
   delete[] cost;
   delete[] buffer;
}

// Destination is set by pooling out the available cells at and around the selected
// destination to provide a unique cell destination for each unit
void Society::set_destination (int destination[3])
{
   const float *map = Map->access_map ();

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   units.size();

   int *cost_indices = scratch;

   int num_units = units.size();

   // Create the selected cost indices equal to the number of units
   cost_function2 (
         map,
         cost,
         cost_indices,
         dim,
         destination,
         num_units,
         buffer);

   int *unit_dest_x = (int*)buffer;
   int *unit_dest_y = unit_dest_x + num_units;
   int *unit_dest_z = unit_dest_y + num_units;

   int ind = 0;
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
   {
      int unit_dest[3];

      (*unit)->get_destination (unit_dest);

      unit_dest_x[ind] = unit_dest[0];
      unit_dest_y[ind] = unit_dest[1];
      unit_dest_z[ind] = unit_dest[2];

      ind++;
   }

   ind = 0;
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
   {
      if (!(*unit)->is_selected()) continue;

      int dest_ind = cost_indices[ind++];

      int dest[3];

      dest[0] = ind_to_i (dest_ind, dim[0], dim[1], dim[2]);
      dest[1] = ind_to_j (dest_ind, dim[0], dim[1], dim[2]);
      dest[2] = ind_to_k (dest_ind, dim[0], dim[1], dim[2]);

      bool found = true;
      for (int unit_ind = 0; unit_ind < num_units; unit_ind++)
      {
         if (
               dest[0] == unit_dest_x[unit_ind] &&
               dest[1] == unit_dest_y[unit_ind] &&
               dest[2] == unit_dest_z[unit_ind]
            )
         {
            found = false;
            break;
         }
      }

      if (found == false) continue;

      (*unit)->set_destination (dest);
   }
}

void Society::update (float time_step)
{

   const float *map = Map->access_map ();

   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
   {
      (*unit)->update (time_step);
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
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++, ind++) {
      x[ind] = (*unit)->get_position (0);
      y[ind] = (*unit)->get_position (1);
      z[ind] = (*unit)->get_position (2);
      selected[ind] = (*unit)->is_selected();
   }

   return ind;
}

void Society::select_units (float *selection_box, int map_layer, bool control_down)
{
   if (control_down == false)
   {
      for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
      {
         (*unit)->unselect();
      }
   }

   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
   {
      float x = (*unit)->get_position (0);
      float y = (*unit)->get_position (1);
      float z = (*unit)->get_position (2);

      // Ignore units that are not at the current map layer
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
         (*unit)->select();
      }
   }
}

void Society::select_all (void)
{
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
      (*unit)->select();
}

void Society::unselect_all (void)
{
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
      (*unit)->unselect();
}
