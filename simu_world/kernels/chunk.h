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
      Chunk( void );

      Chunk( unsigned int  id_in,
             int           abs_pos_id_in[3],
             int          *chunk_dim_in,
             float        *position_in);

      ~Chunk( void );

      // Functions
      int  get_position(float *position_out, int block_index);
      int  get_block( int *block_index);
      void change(float *position_in);
      unsigned int get_dimensions(void);
      unsigned int get_dimensions( unsigned int *dimensions_out);
      void display_info( void);
      bool position_in_chunk( float *poisition_in);
      void move( int x, int y, int z);
      void update( void );
      void get_abs_pos_id( int *abs_pos_id );
      bool is_valid( void );
      void set_color( float *color_in );
      void get_color( float *color_out );

   private:

      // Parameters:
      int         *blocks;
      unsigned int chunk_dim[3];
      unsigned int id;
      float        position[3];
      int          abs_pos_id[3];
      int          prev_abs_pos_id[3];
      float        color[3];
      bool         valid;
      bool         changed;
      bool         first_populated;

      // Functions:
      void create_random( void );
      void create_flat( void );
      void generate_chunk( void );
      void write_chunk( void );

};

/*
** class name: Node
*/
class Node
{
   public:
      Chunk* chunk;

      Node* front;
      Node* back;
      Node* left;
      Node* right;
      Node* top;
      Node* bottom;
      Node* next;
};

/*
** class name: Chunks_new
*/
class Chunks_new
{
   public:

      Chunks_new( int* size );
     ~Chunks_new( void );

   private:

      int size[3];
      Node* current_node;
      Node* base_node;

};

#endif
