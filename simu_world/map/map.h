#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <pthread.h>
#include "opengl_interface.h"
#include "queue.h"
#include "workspace.h"

/*
** class name: Map
*/
class Map
{
   public:

      Map( pthread_barrier_t*,
           pthread_barrier_t* );

     ~Map( void );

      void update( void );

      void map_shift( float* position );

      void render_chunk( float* user_position,
                         float* user_direction,
                         float  window_distance,
                         float  window_width );

      int  get_abs_element( int* position );

      void set_phys_chunk_color( int    p_ind_x,
                                 int    p_ind_y,
                                 int    p_ind_z,
                                 float* color );

      void diagnostics( int* position );
      Queue* queue;

      pthread_barrier_t* IO_barrier;
      pthread_barrier_t* update_barrier;

   private:

      Workspace workspace;
      ReadQueue<int> readQueue;

      void shift( int x, int y, int z);

      int   num_chunks_x;
      int   num_chunks_y;
      int   num_chunks_z;
      bool* write_permissions;

      void get_physical_chunk_position( int* abs_position,
                                        int* physical_chunk_position );

      void get_relative_element_position( int* position_in,
                                          int* element_position );

      void set_chunk( int* src,
                      int  chunk_x,
                      int  chunk_y,
                      int  chunk_z );

      void get_chunk( int* dst,
                      int  chunk_x,
                      int  chunk_y,
                      int  chunk_z,
                      int  aug );

      int *physical_chunk_id_x,
          *physical_chunk_id_y,
          *physical_chunk_id_z;

      int *virtual_chunk_id_x,
          *virtual_chunk_id_y,
          *virtual_chunk_id_z;

      int* blocks;
      int* io_ids;
      int  dim_x;
      int  dim_y;
      int  dim_z;

      int chunk_dim_x;
      int chunk_dim_y;
      int chunk_dim_z;

      float map_pos_x;
      float map_pos_y;
      float map_pos_z;

      float* vertices;
};

#endif
