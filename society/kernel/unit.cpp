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
      std::vector<Unit*> &all_units_in,
      float *scratch) :
   position_x (position_x_in), position_y (position_y_in), position_z (position_z_in)
{
   speed     = 0.0f;
   direction = 0.0f;

   Map = Map_in;
   all_units = all_units_in;

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

void Unit::update (float time_step)
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

      // Iditify if the select destination is available. Otherwise, find one that is
      bool conflicts = false;
      do {
         conflicts = false;

         for (std::vector<Unit*>::iterator it = all_units.begin(); it != all_units.end(); it++)
         {
            if (*it == this) continue;

            (*it)->get_destination (local_dest);

            int map_ind =
               dest[2] * dim[0] * dim[1] +
               dest[1] * dim[0]          +
               dest[0];

            // If the select destination conflicts with an
            // existing one, change the destination to a
            // location that is available
            if (
                  (map[map_ind] < 0.0f ||
                   (local_dest[0] == dest[0] &&
                    local_dest[1] == dest[1] &&
                    local_dest[2] == dest[2])))
            {
               conflicts = true;

               dest[0]++;

               if (dest[0] >= dim[0]) {
                  dest[0] = 0;
                  dest[1]++;
               }
               if (dest[1] >= dim[1]) {
                  dest[1] = 0;
                  dest[2]++;
               }
               if (dest[2] >= dim[2]) {
                  dest[0] = dest[1] = dest[2] = 0;
               }
            }
         }
      } while (conflicts);

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

   // speed = dist / time, dist = speed * time
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
