#include <iostream>
#include "map.h"
#include "unit.h"
#include <cmath>
#include "pathfinding.h"

Unit::Unit (float position_x_in, float position_y_in, float position_z_in, MAP *Map_in,
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
}

Unit::~Unit (void)
{
   delete[] cost;
   delete[] buffer;
}

void Unit::set_speed (float speed_in)
{
   speed = speed_in;
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

   int x_block = (path[0] % dim[0]);
   int y_block = (path[0] % (dim[0] * dim[1])) / dim[0];
   int z_block = path[0] / (dim[0] * dim[1]);

   float dist2 =
      (position_x - dest[0]) * (position_x - dest[0]) +
      (position_y - dest[1]) * (position_y - dest[1]) +
      (position_z - dest[2]) * (position_z - dest[2]);

std::cout << "block = " << x_block << ", " << y_block << ", " << z_block << std::endl;

   float local_dest[3];

   if (start[0] == dest[0] && start[1] == dest[1] && start[2] == dest[2])
   {
      if (dist2 < 0.01f) {
         speed = 0.0f;
      }
      local_dest[0] = (float)dest[0] + 0.5f;
      local_dest[1] = (float)dest[1] + 0.5f;
      local_dest[2] = (float)dest[2] + 0.5f;
   }
   else {
      local_dest[0] = (float)x_block + 0.5f;
      local_dest[1] = (float)y_block + 0.5f;
      local_dest[2] = (float)z_block + 0.5f;
   }

   if (local_dest[0] > position_x) {
      if (local_dest[1] > position_y) {
         direction = 0.785f;
      }
      else if (local_dest[1] < position_y) {
         direction = 5.498f;
      }
      else {
         direction = 0.0f;
      }
   }
   else if (local_dest[0] < position_x) {
      if (local_dest[1] > position_y) {
         direction = 2.356f;
      }
      else if (local_dest[1] < position_y) {
         direction = 3.927f;
      }
      else {
         direction = 3.142f;
      }
   }
   else {
      if (local_dest[1] > position_y) {
         direction = 1.571f;
      }
      else if (local_dest[1] < position_y) {
         direction = 4.712f;
      }
      else {
         speed = 0.0f;
      }
   }

   position_x += speed * cosf (direction) * time_step;
   position_y += speed * sinf (direction) * time_step;
   // TODO: position_z update
}
