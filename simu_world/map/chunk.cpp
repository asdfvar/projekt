#include "map.h"
#include <iostream>
#include <cstdlib>

Chunk::Chunk( void ) {}

/*
** constructor name: Chunk 
*/
Chunk::Chunk( unsigned int  id_in,
              int           abs_pos_id_in[3],
              int          *chunk_dim_in,
              float        *position_in)
{

   abs_pos_id[0] = abs_pos_id_in[0];
   abs_pos_id[1] = abs_pos_id_in[1];
   abs_pos_id[2] = abs_pos_id_in[2];

   prev_abs_pos_id[0] = abs_pos_id[0];
   prev_abs_pos_id[1] = abs_pos_id[1];
   prev_abs_pos_id[2] = abs_pos_id[2];

   prev_abs_pos_id_new[0] = abs_pos_id[0];
   prev_abs_pos_id_new[1] = abs_pos_id[1];
   prev_abs_pos_id_new[2] = abs_pos_id[2];

   prev_abs_pos_id_new[0] = abs_pos_id[0];
   prev_abs_pos_id_new[1] = abs_pos_id[1];
   prev_abs_pos_id_new[2] = abs_pos_id[2];

   for (int ind = 0; ind < 3; ind++)
   {
      if (chunk_dim_in[ind] % 2 == 0)
      {
         std::cout << "Chunk dimension must be odd" << std::endl;
      }
   }

   for (int ind = 0; ind < 3; ind++) chunk_dim[ind] = chunk_dim_in[ind];

   blocks = new int[chunk_dim[0] * chunk_dim[1] * chunk_dim[2]];

   id = id_in;

   for (int ind = 0; ind < 3; ind++) position[ind] = position_in[ind];

   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   valid           = false;
   changed         = false;
   first_populated = true;

   /*
   ** Attempt to read from file upon creation
   */
   generate_chunk();
   valid      = true;
   reassigned = false;

}

/*
** function name: create_random from: Chunk 
*/
void Chunk::create_random( void )
{
   int chunk_dim_tot = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
   for (int ind = 0; ind < chunk_dim_tot; ind++)
   {
      int random_num = std::rand() % 100;
      if (random_num == 0)
         blocks[ind] = 1;
      else
         blocks[ind] = 0;
   }
}

/*
** function name: create_flat from: Chunk 
*/
void Chunk::create_flat( void )
{
   int chunk_dim_tot = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
   if (abs_pos_id[2] < 0)
   {
      for (int ind = 0; ind < chunk_dim_tot; ind++)
      {
         blocks[ind] = 1;
      }
   }
   else
   {
      for (int ind = 0; ind < chunk_dim_tot; ind++)
      {
         blocks[ind] = 0;
      }
   }
}

/*
** function name: create_solid from: Chunk 
*/
void Chunk::create_solid( void )
{
   int chunk_dim_tot = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];

   for (int ind = 0; ind < chunk_dim_tot; ind++)
   {
      blocks[ind] = 1;
   }
}

/*
** function name: display_info from: Chunk
*/
void Chunk::display_info( void)
{
   bool block_exists = false;
   float l_position[3];
   for (int ind = 0; ind < chunk_dim[0] * chunk_dim[1] * chunk_dim[2]; ind++)
   {
      if (blocks[ind] > 0)
      {
         block_exists = true;
         int stat = get_position( l_position, ind);
         if (stat != 0)
         {
            std::cout << "block position = " << l_position[0] << ", "
                                             << l_position[1] << ", "
                                             << l_position[2] << std::endl;
         }
      }
      
   }

   if (block_exists)
   {
      std::cout << "block exists" << std::endl;
      std::cout << "Chunk position = " << position[0] << ", "
                                       << position[1] << ", "
                                       << position[2] << std::endl;
   }
}

/*
** function name: position_in_chunk from Chunk 
**
** returns true if the position is inside the Chunk 
** with the boundaries defined as:
** [ , ) x [ , ) x [ , )
*/
bool Chunk::position_in_chunk( float *position_in)
{
   if (position_in[0] >= position[0]                                    &&
       position_in[0] <  position[0] + static_cast<float>(chunk_dim[0]) &&
       position_in[1] >= position[1]                                    &&
       position_in[1] <  position[1] + static_cast<float>(chunk_dim[1]) &&
       position_in[2] >= position[2]                                    &&
       position_in[2] <  position[2] + static_cast<float>(chunk_dim[2]))
   {
      return true;
   }

   return false;
}

void Chunk::save_prev_abs_pos_id( void )
{
   prev_abs_pos_id_new[0] = abs_pos_id[0];
   prev_abs_pos_id_new[1] = abs_pos_id[1];
   prev_abs_pos_id_new[2] = abs_pos_id[2];
}

/*
** destructor name: ~Chunk
*/
Chunk::~Chunk(void)
{

   delete[] blocks;

}

/*
** function name: get_dimension from: Chunk
*/
unsigned int Chunk::get_dimension( unsigned int dim )
{
   return chunk_dim[dim];
}

/*
** function name: get_position from Chunk
*/
int Chunk::get_position( float* position_out,
                         int    block_index)
{

   if ( position_out == NULL ) return blocks[block_index];

   if (block_index >= chunk_dim[0] * chunk_dim[1] * chunk_dim[2])
   {
      std::cout << "block index exceeds chunk dimensions" << std::endl;
      return 0;
   }

   // get block x, y, z indices
   unsigned int block_index_x = block_index  %  chunk_dim[0];
   unsigned int block_index_y = (block_index % (chunk_dim[0] * chunk_dim[1])) / chunk_dim[0];
   unsigned int block_index_z = block_index  / (chunk_dim[0] * chunk_dim[1]);

   // get relative position to this block
   float rel_block_pos_x = static_cast<float>(block_index_x);
   float rel_block_pos_y = static_cast<float>(block_index_y);
   float rel_block_pos_z = static_cast<float>(block_index_z);

   // convert to absolute position
   float block_pos_x = rel_block_pos_x + position[0];
   float block_pos_y = rel_block_pos_y + position[1];
   float block_pos_z = rel_block_pos_z + position[2];

   position_out[0] = block_pos_x + 0.5f;
   position_out[1] = block_pos_y + 0.5f;
   position_out[2] = block_pos_z + 0.5f;

   return blocks[block_index];

}

/*
** function name: get_position from Chunk
*/
int Chunk::get_position( float* position_out,
                         int*   block_index)
{
   int index = block_index[2] * chunk_dim[0] * chunk_dim[1] +
               block_index[1] * chunk_dim[0]                +
               block_index[0];

   return get_position( position_out, index );
}

/*
** function name: get_block from Chunk
*/
int Chunk::get_block( int *block_index_in)
{
   if (block_index_in[0] * block_index_in[1] * block_index_in[2] >=
       chunk_dim[0] * chunk_dim[1] * chunk_dim[2])
   {
      std::cout << "block index exceeds chunk dimensions" << std::endl;
      return 0;
   }

   return blocks[ block_index_in[2] * chunk_dim[2] * chunk_dim[1] +
                  block_index_in[1] * chunk_dim[0]                +
                  block_index_in[0] ];
}

/*
** function name: get_abs_pos_id from Chunk
*/
void Chunk::get_abs_pos_id( int *abs_pos_id_out )
{
   abs_pos_id_out[0] = abs_pos_id[0];
   abs_pos_id_out[1] = abs_pos_id[1];
   abs_pos_id_out[2] = abs_pos_id[2];
}

/*
** function name: is_valid from Chunk
*/
bool Chunk::is_valid( void )
{
   return valid;
}

void Chunk::set_color( float *color_in )
{
   color[0] = color_in[0];
   color[1] = color_in[1];
   color[2] = color_in[2];
}

void Chunk::get_color( float *color_out )
{
   color_out[0] = color[0];
   color_out[1] = color[1];
   color_out[2] = color[2];
}
