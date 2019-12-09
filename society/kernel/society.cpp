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

void Society::set_destination (int destination[3])
{
   const float *map = Map->access_map ();

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++) {
      (*unit)->set_destination (destination);
   }
}

// TODO: check timing. What is causing the slow down?
void Society::update (float time_step)
{
   int num_units = units.size();

   // Create the list of destinations determined by other units
   int *unit_dests[3];
   unit_dests[0] = scratch + 0 * num_units;
   unit_dests[1] = scratch + 1 * num_units;
   unit_dests[2] = scratch + 2 * num_units;

   // TODO: modify the path-finding algorithms with the appropriate functionality so that the destination can be set uniquely for each unit

   int this_unit_num = 0;
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
   {

      int unit_num = 0;
      for (std::vector<Unit*>::iterator l_unit = units.begin();
            l_unit != units.end();
            l_unit++)
      {
         int local_location[3];
         (*l_unit)->get_destination (local_location);

         unit_dests[0][unit_num] = local_location[0];
         unit_dests[1][unit_num] = local_location[1];
         unit_dests[2][unit_num] = local_location[2];

         unit_num++;
      }

      int local_dest[3];
      bool conflicts = true;

      int dim[3];

      dim[0] = Map->map_dim (0);
      dim[1] = Map->map_dim (1);
      dim[2] = Map->map_dim (2);

      int min_dist = dim[0] * dim[1] + dim[2] + 1;

      const float *map = Map->access_map ();

      int final_dest[3];

      int dest[3];
      (*unit)->get_destination (dest);

      // Identify if the select destination is available. Otherwise, find one that is
      for (int radius = 0; conflicts; radius++)
      {
         int cell[3];
         for (cell[2] = dest[2] - radius; cell[2] <= dest[2] + radius; cell[2]++) {

            if (cell[2] < 0) continue;

            for (cell[1] = dest[1] - radius; cell[1] <= dest[1] + radius; cell[1]++) {

               if (cell[1] < 0) continue;

               for (cell[0] = dest[0] - radius; cell[0] <= dest[0] + radius; cell[0]++) {

                  if (cell[0] < 0) continue;

                  if (cell[0] > dest[0] - radius && cell[0] < dest[0] + radius &&
                        cell[1] > dest[1] - radius && cell[1] < dest[1] + radius &&
                        cell[2] > dest[2] - radius && cell[2] < dest[2] + radius)
                  {
                     continue;
                  }

                  int map_ind =
                     cell[2] * dim[0] * dim[1] +
                     cell[1] * dim[0]          +
                     cell[0];

                  if (map[map_ind] < 0.0f) continue;

                  conflicts = false;

                  // test if the select cell is already set as a destination for another unit
                  for (unit_num = 0; unit_num < num_units; unit_num++)
                  {
                     if (unit_num == this_unit_num) continue;

                     if ( (unit_dests[0][unit_num] == cell[0] &&
                              unit_dests[1][unit_num] == cell[1] &&
                              unit_dests[2][unit_num] == cell[2]))
                     {
                        conflicts = true;
                        break;
                     }
                  }

                  if (conflicts) continue;

                  bool solution_found = cost_function (
                        map,
                        cost,
                        dim,
                        dest,
                        cell,
                        buffer);

                  int *path = (int*)buffer;

                  int path_size = pathfinding (
                        cost,
                        dim,
                        cell,
                        dest,
                        path);

                  if (path_size < min_dist) {

                     final_dest[0] = cell[0];
                     final_dest[1] = cell[1];
                     final_dest[2] = cell[2];

                     min_dist = path_size;
                  }
               }
            }
         }
      }

      dest[0] = final_dest[0];
      dest[1] = final_dest[1];
      dest[2] = final_dest[2];

      (*unit)->set_destination (dest);

      (*unit)->update (units, time_step);

      this_unit_num++;
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
      x[ind] = (*unit)->get_position_x();
      y[ind] = (*unit)->get_position_y();
      z[ind] = (*unit)->get_position_z();
      selected[ind] = (*unit)->get_selected();
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
      float x = (*unit)->get_position_x();
      float y = (*unit)->get_position_y();
      float z = (*unit)->get_position_z();

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
