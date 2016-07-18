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

   Map = new Map[local_grid_size[0] *
                 local_grid_size[1] *
                 local_grid_size[2]];
}

Map_grid::~Map_grid(void)
{
   delete[] Map;
}
