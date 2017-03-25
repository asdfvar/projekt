#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <vector>
#include "opengl_interface.h"
#include "user.h"
#include "queue.h"

/*
** class name: Chunk
*/
class Chunk
{
   public:
      Chunk( void );

      Chunk( unsigned int  id_in,
             int           abs_pos_id_in[3],
             int          *chunk_dim_in,
             float        *position_in);

      ~Chunk( void );

      // Functions
      int  get_position( float* position_out, int block_index );
      int  get_position( float* position_out, int* block_index );
      int  get_block( int *block_index );
      void change(float *position_in );
      unsigned int get_dimension( unsigned int dim );
      void display_info( void );
      bool position_in_chunk( float *poisition_in );
      void move( int x, int y, int z, Queue* );
      void update( Queue* );
      void populate( Queue* );
      void get_abs_pos_id( int *abs_pos_id );
      bool is_valid( void );
      void set_color( float *color_in );
      void get_color( float *color_out );
      void save_prev_abs_pos_id( void );

      void write_chunk( void );
      bool changed;
      bool changed_new;

   private:

      // Parameters:
      int         *blocks;
      unsigned int chunk_dim[3];
      unsigned int id;
      float        position[3];
      int          abs_pos_id[3];
      int          prev_abs_pos_id[3];
      int          prev_abs_pos_id_new[3];
      float        color[3];
      bool         deleting_chunk;
      bool         valid;
      bool         first_populated;
      bool         reassigned;

      // Functions:
      void generate_chunk( void );
      void create_random( void );
      void create_flat( void );
      void create_solid( void );

};

#endif
