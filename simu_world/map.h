#ifndef MAP_H
#define MAP_H

#include <iostream>

class Map
{
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
   ** 0x1 up-ramp
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
   unsigned char *characteristics;

   const unsigned int map_dim;
   unsigned int id;

   Map( unsigned int id_in );
   ~Map( void );

};

class Map_grid
{
   size_t local_grid_size[3];

   Map *map;

   Map_grid(void);
   ~Map_grid(void);
};

#endif
