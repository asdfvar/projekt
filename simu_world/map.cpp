#include "map.h"

Map::Map(unsigned int id_in)
{
   blocks          = new char[map_dim * map_dim * map_dim];
   characteristics = new char[map_dim * map_dim * map_dim];

   id = id_in;
}

Map::~Map(void)
{
   delete[] blocks;
   delete[] characteristics;
}

Map_grid(void)
{
   Map = new Map[local_grid_size[0] *
                 local_grid_size[1] *
                 local_grid_size[2]]
}
