#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include "opengl_interface.h"
#include "user.h"

/*
** class name: Chunk
*/
class Chunk
{
   public:
      Chunk( void);
      Chunk( unsigned int id_in, int *chunk_dim_in, float *position_in);
      ~Chunk( void );

      // Functions
      int  get_position(float *position_out, int block_index);
      void change(float *position_in);
      unsigned int get_dimensions(void);
      void display_info( void);
      bool position_in_chunk( float *poisition_in);
      void move( int x, int y, int z);

   private:

      // Parameters:
      int         *blocks;
      unsigned int chunk_dim[3];
      unsigned int id;
      float        position[3];

      // Functions:
      void create_random( void);

};

/*
** class name: Map
*/
class Map
{
   public:
      Map(void);
     ~Map(void);

      void update( float *position);
      void render_chunk( User *user);

      Chunk *access_chunk(int v_id_x,
                      int v_id_y,
                      int v_id_z);

      int get_grid_size(int ind);

   private:
      void shift( int x, int y, int z);

      int local_grid_size[3];

      int *virtual_grid_id_x,
          *virtual_grid_id_y,
          *virtual_grid_id_z;

      std::vector<Chunk*> chunks;

};

#endif
