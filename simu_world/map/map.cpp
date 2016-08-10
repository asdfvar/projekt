#include "map.h"
#include <iostream>
#include <cstdlib>

Map::Map( unsigned int id_in, int *map_dim_in, float *position_in)
{
   for (int ind = 0; ind < 3; ind++) if (map_dim_in[ind] % 2 == 0) std::cout << "map dimension must be odd" << std::endl;
   for (int ind = 0; ind < 3; ind++) map_dim[ind] = map_dim_in[ind];
   blocks = new int[map_dim[0] * map_dim[1] * map_dim[2]];

   id = id_in;
   for (int ind = 0; ind < 3; ind++) position[ind] = position_in[ind];

   create_random();
}

void Map::change_position(float *position_in)
{
   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];
}

void Map::create_random( void)
{
   for (int ind = 0; ind < map_dim[0] * map_dim[1] * map_dim[2]; ind++) {
      int random_num = std::rand() % 100;
      if (random_num == 1)
         blocks[ind] = 1;
      else
         blocks[ind] = 0;
   }
}

Map::~Map(void)
{
   delete[] blocks;
}

Map_grid::Map_grid(void)
{

   local_grid_size[0] = 11; // must be odd
   local_grid_size[1] = 11; // must be odd
   local_grid_size[2] = 11; // must be odd

   virtual_grid_id_x = new int[local_grid_size[0]];
   virtual_grid_id_y = new int[local_grid_size[1]];
   virtual_grid_id_z = new int[local_grid_size[2]];

   for (int k = 0; k < local_grid_size[0]; k++) virtual_grid_id_x[k] = k;
   for (int k = 0; k < local_grid_size[1]; k++) virtual_grid_id_y[k] = k;
   for (int k = 0; k < local_grid_size[2]; k++) virtual_grid_id_z[k] = k;

   int map_dim[3] = {65, 65, 65};
   for (int ind_x = -local_grid_size[0]/2, ind = 0; ind_x <= local_grid_size[0]/2; ind_x++)
   {
      for (int ind_y = -local_grid_size[1]/2; ind_y <= local_grid_size[1]/2; ind_y++)
      {
         for (int ind_z = -local_grid_size[2]/2; ind_z <= local_grid_size[2]/2; ind_z++, ind++)
         {
           float position[3] = {(float)(ind_x * map_dim[0]),
                                (float)(ind_y * map_dim[1]),
                                (float)(ind_z * map_dim[2])};

            maps.push_back(Map(ind, map_dim, position));
         }
      }
   }

}

Map_grid::~Map_grid(void)
{
   std::size_t total_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];
   maps.erase (maps.begin(), maps.begin() + total_grid_size);

   delete[] virtual_grid_id_x;
   delete[] virtual_grid_id_y;
   delete[] virtual_grid_id_z;
}

/*
** Circular shift the cube in the specified direction
*/
void Map_grid::shift( int x, int y, int z)
{
   for (int k = 0; k < local_grid_size[0]; k++) {
      for (virtual_grid_id_x[k] -= x;
           virtual_grid_id_x[k] < 0;
           virtual_grid_id_x[k] += local_grid_size[0]) { /* N/A */};
   }
   for (int k = 0; k < local_grid_size[1]; k++) {
      for (virtual_grid_id_y[k] -= y;
           virtual_grid_id_y[k] < 0;
           virtual_grid_id_y[k] += local_grid_size[1]) { /* N/A */};
   }
   for (int k = 0; k < local_grid_size[2]; k++) {
      for (virtual_grid_id_z[k] -= z;
           virtual_grid_id_z[k] < 0;
           virtual_grid_id_z[k] += local_grid_size[2]) { /* N/A */};
   }
}

