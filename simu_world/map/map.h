#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <iostream>

/*
** class name: Map
*/
class Map
{
   public:
      Map( void);
      Map( unsigned int id_in, int *map_dim_in, float *position_in);
      ~Map( void );

      // Functions
      int  get_position(float *position_out, int block_index);
      void change_position(float *position_in);
      unsigned int get_dimensions(void);

   private:

      // Parameters:
      int *blocks;
      unsigned int map_dim[3];
      unsigned int id;
      float position[3];

      // Functions:
      void create_random( void);

};

/*
** class name: Map_grid
*/
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
