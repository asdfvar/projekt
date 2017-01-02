#include "map.h"
#include <iostream>
#include <cstdlib>
#include "draw_block.h"

/*
** constructor name: Chunk 
*/
Chunk::Chunk( unsigned int id_in, int *chunk_dim_in, float *position_in)
{
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

   valid = false;

}

void Chunk::update( void )
{
   if (!valid)
   {
      create_random();
      valid = true;
   }
}

/*
** function name: create_random from: Chunk 
*/
void Chunk::create_random( void)
{
   for (int ind = 0; ind < chunk_dim[0] * chunk_dim[1] * chunk_dim[2]; ind++)
   {
      int random_num = std::rand() % 100;
      if (random_num == 0)
         blocks[ind] = 1;
      else
         blocks[ind] = 0;
   }
}

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
   if (position_in[0] >= position[0]                                  &&
       position_in[0] <  position[0] + static_cast<float>(chunk_dim[0]) &&
       position_in[1] >= position[1]                                  &&
       position_in[1] <  position[1] + static_cast<float>(chunk_dim[1]) &&
       position_in[2] >= position[2]                                  &&
       position_in[2] <  position[2] + static_cast<float>(chunk_dim[2]))
   {
      return true;
   }

   return false;
}

/*
** function name: move from Chunk 
**
** move the chunk a discrete number of chunks
*/
void Chunk::move( int x, int y, int z)
{
   position[0] += static_cast<float>(x) * chunk_dim[0];
   position[1] += static_cast<float>(y) * chunk_dim[1];
   position[2] += static_cast<float>(z) * chunk_dim[2];

   valid = false;
}

/*
** destructor name: ~Chunk
*/
Chunk::~Chunk(void)
{
   delete[] blocks;
}

/*
** function name: get_dimensions from Chunk 
*/
unsigned int Chunk::get_dimensions(void)
{
   return chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
}

/*
** function name: get_position from Chunk
*/
int Chunk::get_position(float *position_out,
                      int    block_index)
{

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