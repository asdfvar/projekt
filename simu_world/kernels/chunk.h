#ifndef CHUNK_H
#define CHUNK_H

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

      Chunk( unsigned int  id_in,
             int           abs_pos_id_in[3],
             int          *chunk_dim_in,
             float        *position_in);

      ~Chunk( void );

      // Functions
      int  get_position(float *position_out, int block_index);
      void change(float *position_in);
      unsigned int get_dimensions(void);
      void display_info( void);
      bool position_in_chunk( float *poisition_in);
      void move( int x, int y, int z);
      void update( void );
      void get_abs_pos_id( int *abs_pos_id );

   private:

      // Parameters:
      int         *blocks;
      unsigned int chunk_dim[3];
      unsigned int id;
      float        position[3];
      bool         valid;
      int          abs_pos_id[3];

      // Functions:
      void create_random( void);

};

#endif
