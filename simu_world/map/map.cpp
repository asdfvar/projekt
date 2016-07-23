#include "map.h"

Map::Map(unsigned int id_in)
{
   map_dim         = 64;
   blocks          = new char[map_dim * map_dim * map_dim];
   characteristics = new char[map_dim * map_dim * map_dim];

   id = id_in;
}

Map::~Map(void)
{
   delete[] blocks;
   delete[] characteristics;
}

Map_grid::Map_grid(void)
{

   local_grid_size[0] = 11;
   local_grid_size[1] = 11;
   local_grid_size[2] = 11;

   std::size_t total_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];

   for (int k = 0; k < total_grid_size; k++)
      maps.push_back(Map(k));

}

Map_grid::~Map_grid(void)
{
   std::size_t total_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];
   maps.erase (maps.begin(), maps.begin() + total_grid_size);
}
