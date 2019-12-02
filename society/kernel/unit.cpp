#include <iostream>
#include "map.h"
#include "unit.h"
#include <cmath>
#include "pathfinding.h"

Unit::Unit (
      float position_x_in,
      float position_y_in,
      float position_z_in,
      MAP *Map_in,
      float *scratch) :
   position_x (position_x_in), position_y (position_y_in), position_z (position_z_in)
{
   speed     = 0.0f;
   direction = 0.0f;

   Map = Map_in;

   int map_dims[3] = { Map->map_dim (0), Map->map_dim (1), Map->map_dim (2) };

   cost   = new float [map_dims[0] * map_dims[1] * map_dims[2]];
   buffer = new float [map_dims[0] * map_dims[1] * map_dims[2]];

   dest[0] = position_x;
   dest[1] = position_y;
   dest[2] = position_z;

   update_path = true;

   max_speed = 40.0f;
}

Unit::~Unit (void)
{
   delete[] cost;
   delete[] buffer;
}

void Unit::set_destination (int dest_in[3])
{
   const float *map = Map->access_map ();

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   int ind =
      dest_in[2] * dim[0] * dim[1] +
      dest_in[1] * dim[0] +
      dest_in[0];

   if (map[ind] < 0.0f) return;

   dest[0] = dest_in[0],
   dest[1] = dest_in[1],
   dest[2] = dest_in[2];
};

void Unit::get_destination (int *dest_out)
{
   dest_out[0] = dest[0];
   dest_out[1] = dest[1];
   dest_out[2] = dest[2];
}

void Unit::update (
      std::vector<Unit*> &all_units,
      float time_step)
{

   int start[3];

   start[0] = (int)position_x;
   start[1] = (int)position_y;
   start[2] = (int)position_z;

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   const float *map = Map->access_map ();

   if (update_path)
   {
      int local_dest[3];
      bool conflicts = true;

      int min_dist = dim[0] * dim[1] + dim[2] + 1;

      int final_dest[3];

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

                  int map_ind =
                     cell[2] * dim[0] * dim[1] +
                     cell[1] * dim[0]          +
                     cell[0];

                  if (map[map_ind] < 0.0f) continue;

                  conflicts = false;

                  // test if the select cell is already set as a destination for another unit
                  for (std::vector<Unit*>::iterator other_unit = all_units.begin();
                        other_unit != all_units.end();
                        other_unit++)
                  {
                     if (*other_unit == this) continue;

                     int other_unit_dest[3];
                     (*other_unit)->get_destination (other_unit_dest);

                     if ( (other_unit_dest[0] == cell[0] &&
                              other_unit_dest[1] == cell[1] &&
                              other_unit_dest[2] == cell[2])) {
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

      // If the select destination conflicts with an
      // existing one, change the destination to a
      // location that is available.

      // The destination has a conflict if it's an invalid
      // cell or there is already another unit that has claimed
      // that cell as its destination.


      // Find the path to the destination
      bool solution_found = cost_function (
            map,
            cost,
            dim,
            dest,
            start,
            buffer);

      int *path = (int*)buffer;

      int path_size = pathfinding (
            cost,
            dim,
            start,
            dest,
            path);

      next_cell[0] = (path[0] % dim[0]);
      next_cell[1] = (path[0] % (dim[0] * dim[1])) / dim[0];
      next_cell[2] = path[0] / (dim[0] * dim[1]);

      update_path = false;
      speed = max_speed;
   }

   float local_dest[3];

   // Condition check if the unit is in the local destination cell,
   // set the local destination to the center of the cell
   if (start[0] == dest[0] && start[1] == dest[1] && start[2] == dest[2])
   {
      local_dest[0] = (float)dest[0] + 0.5f;
      local_dest[1] = (float)dest[1] + 0.5f;
      local_dest[2] = (float)dest[2] + 0.5f;
   }
   else {
      local_dest[0] = (float)next_cell[0] + 0.5f;
      local_dest[1] = (float)next_cell[1] + 0.5f;
      local_dest[2] = (float)next_cell[2] + 0.5f;
   }

   float dist2 =
      (position_x - local_dest[0]) * (position_x - local_dest[0]) +
      (position_y - local_dest[1]) * (position_y - local_dest[1]) +
      (position_z - local_dest[2]) * (position_z - local_dest[2]);

   // set the position to the local destination if the projection would otherwise
   // surpass it. Then signal to update the path
   float comp_dist = speed * time_step;
   if (dist2 <= comp_dist * comp_dist)
   {
      position_x = local_dest[0];
      position_y = local_dest[1];
      position_z = local_dest[2];
      
      speed = max_speed;
      update_path = true;
      return;
   }

   float step[3];

   if (local_dest[0] > position_x) {
      if (local_dest[1] > position_y) {
         float costh = 0.7071068f;
         float sinth = 0.7071068f;
         step[0] = speed * costh * time_step;
         step[1] = speed * sinth * time_step;

         if (step[0] > local_dest[0] - position_x) {
            position_x = local_dest[0];
         } else {
            position_x += step[0];
         }

         if (step[1] > local_dest[1] - position_y) {
            position_y = local_dest[1];
         } else {
            position_y += step[1];
         }
      }
      else if (local_dest[1] < position_y) {
         float costh =  0.7071068f;
         float sinth = -0.7071068f;
         step[0] = speed * costh * time_step;
         step[1] = speed * sinth * time_step;

         if (step[0] > local_dest[0] - position_x) {
            position_x = local_dest[0];
         } else {
            position_x += step[0];
         }

         if (step[1] < local_dest[1] - position_y) {
            position_y = local_dest[1];
         } else {
            position_y += step[1];
         }
      }
      else {
         step[0] = speed * time_step;

         if (step[0] > local_dest[0] - position_x) {
            position_x = local_dest[0];
         } else {
            position_x += step[0];
         }

      }
   }
   else if (local_dest[0] < position_x) {
      if (local_dest[1] > position_y) {
         float costh = -0.7071068f;
         float sinth =  0.7071068f;
         step[0] = speed * costh * time_step;
         step[1] = speed * sinth * time_step;

         if (step[0] < local_dest[0] - position_x) {
            position_x = local_dest[0];
         } else {
            position_x += step[0];
         }

         if (step[1] > local_dest[1] - position_y) {
            position_y = local_dest[1];
         } else {
            position_y += step[1];
         }
      }
      else if (local_dest[1] < position_y) {
         float costh = -0.7071068f;
         float sinth = -0.7071068f;
         step[0] = speed * costh * time_step;
         step[1] = speed * sinth * time_step;

         if (step[0] < local_dest[0] - position_x) {
            position_x = local_dest[0];
         } else {
            position_x += step[0];
         }

         if (step[1] < local_dest[1] - position_y) {
            position_y = local_dest[1];
         } else {
            position_y += step[1];
         }
      }
      else {
         step[0] = -speed * time_step;

         if (step[0] < local_dest[0] - position_x) {
            position_x = local_dest[0];
         } else {
            position_x += step[0];
         }

      }
   }
   else {
      if (local_dest[1] > position_y) {
         step[1] = speed * time_step;

         if (step[1] > local_dest[1] - position_y) {
            position_y = local_dest[1];
         } else {
            position_y += step[1];
         }
      }
      else if (local_dest[1] < position_y) {
         step[1] = -speed * time_step;

         if (step[1] < local_dest[1] - position_y) {
            position_y = local_dest[1];
         } else {
            position_y += step[1];
         }
      }
      else {
         speed = 0.0f;
      }
   }

   // TODO: position_z update
}
