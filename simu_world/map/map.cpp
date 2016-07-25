#include "map.h"
#include <cstdlib>

Map::Map(unsigned int id_in)
{
   map_dim[0]      = 65;
   map_dim[1]      = 65;
   map_dim[2]      = 65;
   blocks          = new char[map_dim[0] * map_dim[1] * map_dim[2]];
   characteristics = new char[map_dim[0] * map_dim[1] * map_dim[2]];

   id = id_in;

   create_random();
}

void Map::create_random( void)
{
   for (int ind = 0; ind < map_dim[0] * map_dim[1] * map_dim[2]; ind++) {
      blocks[ind] = static_cast<char>( std::rand() % 2);
      characteristics[ind] = 0;
   }
}

Map::~Map(void)
{
   delete[] blocks;
   delete[] characteristics;
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

   std::size_t total_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];

   for (int k = 0; k < total_grid_size; k++)
      maps.push_back(Map(k));

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

