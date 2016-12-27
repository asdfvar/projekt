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

   create_random();
}

/*
** function name: change_position from: Chunk 
** TODO: obsolete
*/
void Chunk::change_position(float *position_in)
{
   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];
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

/*
** constructor name: Map_grid
*/
Map_grid::Map_grid(void)
{

   local_grid_size[0] = 13;
   local_grid_size[1] = 13;
   local_grid_size[2] = 13;
   int total_local_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];

   virtual_grid_id_x = new int[local_grid_size[0]];
   virtual_grid_id_y = new int[local_grid_size[1]];
   virtual_grid_id_z = new int[local_grid_size[2]];

   for (int k = 0; k < local_grid_size[0]; k++) virtual_grid_id_x[k] = k;
   for (int k = 0; k < local_grid_size[1]; k++) virtual_grid_id_y[k] = k;
   for (int k = 0; k < local_grid_size[2]; k++) virtual_grid_id_z[k] = k;

   chunks.reserve( total_local_grid_size);
   std::vector<Chunk*>::size_type sz = chunks.capacity();

   int chunk_dim[3] = {5, 5, 5};
   int ind;
   for (int ind_z = -local_grid_size[2]/2, ind = 0; ind_z <= local_grid_size[2]/2; ind_z++)
   {
      for (int ind_y = -local_grid_size[1]/2; ind_y <= local_grid_size[1]/2; ind_y++)
      {
         for (int ind_x = -local_grid_size[0]/2; ind_x <= local_grid_size[0]/2; ind_x++, ind++)
         {
           float position[3] = {(float)(ind_x * chunk_dim[0]),
                                (float)(ind_y * chunk_dim[1]),
                                (float)(ind_z * chunk_dim[2])};

           chunks.push_back( new Chunk(ind, chunk_dim, position));

           if (sz < chunks.capacity())
           {
              std::cout << "Chunk grid capacity increased" << std::endl;
              sz = chunks.capacity();
           }

           if (ind % 100 == 0)
           {
              std::cout << (float)ind / (float)total_local_grid_size << std::endl;
           }
         }
      }
   }

}

/*
** destructor name: ~Map_grid
*/
Map_grid::~Map_grid(void)
{
   std::size_t total_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];
   chunks.erase (chunks.begin(), chunks.begin() + total_grid_size);

   delete[] virtual_grid_id_x;
   delete[] virtual_grid_id_y;
   delete[] virtual_grid_id_z;
}

void Map_grid::update( float *position)
{
   int virtual_grid[3] = { -1, -1, -1 };

   for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
   {
      for (int y_dir = 0; y_dir < local_grid_size[0]; y_dir++)
      {
         for (int z_dir = 0; z_dir < local_grid_size[0]; z_dir++)
         {

            Chunk *this_chunk = access_chunk( x_dir,
                                              y_dir,
                                              z_dir);

            if (this_chunk->position_in_chunk( position))
            {
               virtual_grid[0] = virtual_grid_id_x[x_dir];
               virtual_grid[1] = virtual_grid_id_y[y_dir];
               virtual_grid[2] = virtual_grid_id_z[z_dir];
            }

         }
      }
   }

   int mid_block[3] = { local_grid_size[0] / 2,
                        local_grid_size[1] / 2,
                        local_grid_size[2] / 2 };

   if (virtual_grid[0] > mid_block[0])
   {
      shift(1, 0, 0);
   }
   if (virtual_grid[0] < mid_block[0])
   {
      shift(-1, 0, 0);
   }
   if (virtual_grid[1] > mid_block[1])
   {
      shift(0, 1, 0);
   }
   if (virtual_grid[1] < mid_block[1])
   {
      shift(0, -1, 0);
   }
   if (virtual_grid[2] > mid_block[2])
   {
      shift(0, 0, 1);
   }
   if (virtual_grid[2] < mid_block[2])
   {
      shift(0, 0, -1);
   }

}

void Map_grid::render_chunk( User *user)
{
   float block_position[3];
   Chunk *chunk;

   for (int grid_x = 0; grid_x < local_grid_size[0]; grid_x++)
   {
      for (int grid_y = 0; grid_y < local_grid_size[1]; grid_y++)
      {
         for (int grid_z = 0; grid_z < local_grid_size[2]; grid_z++)
         {
            chunk = access_chunk( grid_x, grid_y, grid_z);

            // iterate through all the blocks
            for (unsigned int block_ind = 0; block_ind < chunk->get_dimensions(); block_ind++)
            {

               if (!chunk->get_position( block_position, block_ind)) continue;

               draw_block( block_position,
                           user);
            }
         }
      }
   }
}

/*
** function name: access_chunk from: Map_grid
** Return pointer to the specified chunk block
*/
Chunk *Map_grid::access_chunk(int p_id_x,
                              int p_id_y,
                              int p_id_z)
{
   if (p_id_x < 0 || p_id_x >= local_grid_size[0] ||
       p_id_y < 0 || p_id_y >= local_grid_size[1] ||
       p_id_z < 0 || p_id_z >= local_grid_size[2])
   {
      std::cout << "select index of " << p_id_x << ", "  << p_id_x << ", "  << p_id_x <<
                   " out of bounds: " << local_grid_size[0] << ", " <<
                                         local_grid_size[1] << ", " <<
                                         local_grid_size[2] << std::endl;
   }

   unsigned int index =  p_id_x + 
                         p_id_y * local_grid_size[0] +
                         p_id_z * local_grid_size[0] * local_grid_size[1];

   return chunks.at( index );
}

/*
** function name: shift from: Map_grid
** Circular shift the cube in the specified direction
*/
void Map_grid::shift( int x, int y, int z)
{

   for (int k = 0; k < local_grid_size[0]; k++)
   {
      
      virtual_grid_id_x[k] -= x;
      if (virtual_grid_id_x[k] >= local_grid_size[0])
      {
         for (;virtual_grid_id_x[k] >= local_grid_size[0];
               virtual_grid_id_x[k] -= local_grid_size[0]) { /* NA */}

         for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( k,
                                                 y_dir,
                                                 z_dir);

               this_chunk->move( -local_grid_size[0], 0, 0);
            }
         }

      }

      if (virtual_grid_id_x[k] < 0)
      {
         for (;virtual_grid_id_x[k] < 0; virtual_grid_id_x[k] += local_grid_size[0]) { /* NA */}

         for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( k,
                                                 y_dir,
                                                 z_dir);

               this_chunk->move( local_grid_size[0], 0, 0);
            }
         }
      }

   }

   for (int k = 0; k < local_grid_size[1]; k++)
   {
      virtual_grid_id_y[k] -= y;
      if (virtual_grid_id_y[k] >= local_grid_size[1])
      {
         for (;virtual_grid_id_y[k] >= local_grid_size[1];
               virtual_grid_id_y[k] -= local_grid_size[1]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 k,
                                                 z_dir);

               this_chunk->move( 0, -local_grid_size[1], 0);
            }
         }

      }

      if (virtual_grid_id_y[k] < 0)
      {
         for (;virtual_grid_id_y[k] < 0; virtual_grid_id_y[k] += local_grid_size[1]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 k,
                                                 z_dir);

               this_chunk->move( 0, local_grid_size[1], 0);
            }
         }
      }


   }

   for (int k = 0; k < local_grid_size[2]; k++)
   {
      virtual_grid_id_z[k] -= z;
      if (virtual_grid_id_z[k] >= local_grid_size[2])
      {
         for (;virtual_grid_id_z[k] >= local_grid_size[2];
               virtual_grid_id_z[k] -= local_grid_size[2]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 y_dir,
                                                 k);

               this_chunk->move( 0, 0, -local_grid_size[2]);
            }
         }

      }

      if (virtual_grid_id_z[k] < 0)
      {
         for (;virtual_grid_id_z[k] < 0; virtual_grid_id_z[k] += local_grid_size[2]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 y_dir,
                                                 k);

               this_chunk->move( 0, 0, local_grid_size[2]);
            }
         }
      }


   }
}

int Map_grid::get_grid_size(int ind)
{
   return local_grid_size[ind];
}
