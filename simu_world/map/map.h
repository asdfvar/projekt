#ifndef MAP_H
#define MAP_H

#include "chunk.h"
#include "chunks.h"
#include <iostream>
#include <pthread.h>
#include "opengl_interface.h"
#include "user.h"
#include "queue.h"
#include "text.h"

/*
** class name: Map
*/
class Map
{
   public:

      Map( pthread_barrier_t* );
     ~Map( void );

      void update( void );
      void map_shift( float* position );
      void render_chunk( User *user );

      Chunk *access_chunk( int v_id_x,
                           int v_id_y,
                           int v_id_z );

      int  get_abs_element( int* position );

      void set_phys_chunk_color( int    p_ind_x,
                                 int    p_ind_y,
                                 int    p_ind_z,
                                 float* color );

      void diagnostics( int* position, Text* text );
      void debug_info( void );
      Queue *queue;

      pthread_barrier_t* IO_barrier;

   private:

      void shift( int x, int y, int z);

      int num_chunks[3];

      void get_physical_chunk_position( int* abs_position,
                                        int* physical_chunk_position );

      void get_relative_element_position( int* position_in,
                                          int* element_position );

      int *physical_chunk_id_x,
          *physical_chunk_id_y,
          *physical_chunk_id_z;

      int *virtual_chunk_id_x,
          *virtual_chunk_id_y,
          *virtual_chunk_id_z;

      Chunks* chunks;

      int* blocks;
      int dim_x;
      int dim_y;
      int dim_z;

      int chunk_dim_x;
      int chunk_dim_y;
      int chunk_dim_z;

      float map_pos_x;
      float map_pos_y;
      float map_pos_z;

      float* vertices;

      float* buf;

};

#endif
