#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <iostream>

class Map
{
   public:
      Map( unsigned int id_in );
      ~Map( void );

   private:

      /*
      ** The blocks that make up this world
      ** 0x0 empty
      ** 0x1 stone
      ** 0x2 N/A
      ** 0x3 N/A
      ** 0x4 N/A
      ** 0x5 N/A
      ** 0x6 N/A
      ** 0x7 N/A
      ** 0x8 N/A
      ** 0x9 N/A
      ** 0xa N/A
      ** 0xb N/A
      ** 0xc N/A
      ** 0xd N/A
      ** 0xe N/A
      ** 0xf N/A
      */
      char *blocks;

      /*
      ** Characteristics of each block.
      ** 0x0 default
      ** 0x1 up/down-ramp
      ** 0x2 N/A
      ** 0x3 N/A
      ** 0x4 N/A
      ** 0x5 N/A
      ** 0x6 N/A
      ** 0x7 N/A
      ** 0x8 N/A
      ** 0x9 N/A
      ** 0xa N/A
      ** 0xb N/A
      ** 0xc N/A
      ** 0xd N/A
      ** 0xe N/A
      ** 0xf N/A
      */
      char *characteristics;

      unsigned int map_dim[3];
      unsigned int id;

      void create_random( void);

};

class Map_grid
{
   public:
      Map_grid(void);
      ~Map_grid(void);

      void shift( int x, int y, int z);

   private:
      std::size_t local_grid_size[3];

      int *virtual_grid_id_x,
          *virtual_grid_id_y,
          *virtual_grid_id_z;

      std::vector<Map> maps;

};

#endif
