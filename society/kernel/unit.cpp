#include <iostream>
#include "map.h"
#include "unit.h"
#include "timer.h"
#include <cmath>
#include "pathfinding.h"

Unit::Unit (
      float position_x_in,
      float position_y_in,
      float position_z_in,
      MAP *Map_in)
{
   direction = 0.0f;

   position[0] = position_x_in;
   position[1] = position_y_in;
   position[2] = position_z_in;

   Map = Map_in;

   int map_dims[3] = { Map->map_dim (0), Map->map_dim (1), Map->map_dim (2) };

   cost    = new float [map_dims[0] * map_dims[1] * map_dims[2]];
   buffer  = new float [map_dims[0] * map_dims[1] * map_dims[2]];
   scratch = new int   [map_dims[0] * map_dims[1] * map_dims[2]];
   path    = new int   [map_dims[0] * map_dims[1] * map_dims[2]];

   dest[0] = position[0];
   dest[1] = position[1];
   dest[2] = position[2];

   selected    = false;

   path_size = 0;

   max_speed = 40.0f;
   speed     = max_speed;
}

Unit::~Unit (void)
{
   delete[] cost;
   delete[] buffer;
   delete[] scratch;
   delete[] path;
}

void Unit::set_destination (int dest_in[3])
{
   if (selected == false) return;

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

   int start[3];

   start[0] = (int)position[0];
   start[1] = (int)position[1];
   start[2] = (int)position[2];

   // Find the path to the destination.
   // The cost function is produced with the destination
   // as the start index for the purpose of descending to the destination
   bool solution_found = cost_function (
         map,
         cost,
         dim,
         dest_in,
         start,
         buffer);

   // Set the destination to the current location if a solution is not found
   if (!solution_found)
   {
      return;
   }

   dest[0] = dest_in[0],
   dest[1] = dest_in[1],
   dest[2] = dest_in[2];

   path_size = pathfinding (
         cost,
         dim,
         start,
         dest,
         path);

   for (int ind = 0; ind < path_size; ind++) path[ind] = path[ind+1];
   if (path_size > 0) path_size--;
};

void Unit::get_destination (int *dest_out)
{
   dest_out[0] = dest[0];
   dest_out[1] = dest[1];
   dest_out[2] = dest[2];
}

void Unit::update (float time_step)
{
   long start0 = startTime ();

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   float elapsed0 = endTime (start0);
   long start1 = startTime ();

   float local_dest[3];

   // Condition check if the unit is in the local destination cell,
   // set the local destination to the center of the cell
   if (path_size == 0)
   {
      local_dest[0] = (float)dest[0] + 0.5f;
      local_dest[1] = (float)dest[1] + 0.5f;
      local_dest[2] = (float)dest[2] + 0.5f;
   }
   else
   {
      int next_cell[3];

      next_cell[0] = (path[0] % dim[0]);
      next_cell[1] = (path[0] % (dim[0] * dim[1])) / dim[0];
      next_cell[2] = path[0] / (dim[0] * dim[1]);

      local_dest[0] = (float)next_cell[0] + 0.5f;
      local_dest[1] = (float)next_cell[1] + 0.5f;
      local_dest[2] = (float)next_cell[2] + 0.5f;
   }

   float dist2 =
      (position[0] - local_dest[0]) * (position[0] - local_dest[0]) +
      (position[1] - local_dest[1]) * (position[1] - local_dest[1]) +
      (position[2] - local_dest[2]) * (position[2] - local_dest[2]);

   float elapsed1 = endTime (start1);
   long start2 = startTime ();

   // set the position to the local destination if the projection would otherwise
   // surpass it
   float comp_dist = speed * time_step;
   if (dist2 <= comp_dist * comp_dist)
   {
      position[0] = local_dest[0];
      position[1] = local_dest[1];
      position[2] = local_dest[2];

      // Remove the traversed path element from the list.
      // Return when the destination has been reached (which happens
      // when the path size is zero)
      for (int ind = 0; ind < path_size; ind++) path[ind] = path[ind+1];
      if (path_size > 0) path_size--;
      if (path_size <= 0) return;

      // Adjust time step to the remaining time
      time_step -= sqrtf (dist2) / speed;
   }

   float step[3];

   float diff_pos[3];

   // Update the position

   diff_pos[0] = local_dest[0] - position[0];
   diff_pos[1] = local_dest[1] - position[1];
   diff_pos[2] = local_dest[2] - position[2];

   float theta = atan2f (diff_pos[1], diff_pos[0]);

   position[0] += speed * cosf (theta) * time_step;
   position[1] += speed * sinf (theta) * time_step;

   theta = atan2f (diff_pos[2], sqrtf (diff_pos[0] * diff_pos[0] + diff_pos[1] * diff_pos[1]));

   position[2] += speed * sinf (theta) * time_step;

   float elapsed2 = endTime (start2);
}
