#include "map.h"
#include "queue.h"

/*
** function name: map_shift from: Map
**
** determine if there is a map shift based from user position.
** when the user is no longer at the center of
** the map, shift the chunks until the user is
** at the center of the map
*/
void Map::map_shift( float *position )
{
   int virtual_grid[3] = { -1, -1, -1 };

#ifdef BLOCKS
   int total_num_chunk_elements = num_chunk_elements[0] *
                                  num_chunk_elements[1] *
                                  num_chunk_elements[2];

   float displacement_x = position[0] - map_pos_x;

   if (displacement_x > chunk_dim_x)
   {
      for (int k = 0; k < num_chunks[2]; k++)
      {
         for (int j = 0; j < num_chunks[1]; j++)
         {
            get_chunk( buf,
                       0,
                       j,
                       k );

            int abs_chunk_x = map_pos_x / num_chunk_elements[0];
            int abs_chunk_y = map_pos_y / num_chunk_elements[1];
            int abs_chunk_z = map_pos_z / num_chunk_elements[2];

            std::string filename = create_filename( abs_chunk_x + 0,
                                                    abs_chunk_y + j,
                                                    abs_chunk_z + k );

            queue->fill_buffer( filename,
                                buf,
                                0,
                                total_num_chunk_elements );
         }
      }

      // Shift the blocks back modulo the chunk_dim_x
      for (int k = 0; k < dim_z; k++)
      {
         for (int j = 0; j < dim_y; j++)
         {
            for (int i = 0; i < chunk_dim_x; i++)
            {
               buf[i] = blocks[ i                 +
                                j * dim_x         +
                                k * dim_x * dim_y ];
            }
      
            for (int i = 0; i < dim_x - chunk_dim_x; i++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y   ] =

                       blocks[ i                 +
                               j * dim_x         +
                               k * dim_x * dim_y +
                               chunk_dim_x         ];
            }

            for (int ind = 0, i = dim_x - chunk_dim_x; i < dim_x; i++, ind++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y ] = buf[ind];
            }
         }
      }

      // move the blocks, as a whole, appropriately
      map_pos_x += (float)chunk_dim_x;
   }

   else if (displacement_x < 0.0f)
   {
      for (int k = 0; k < num_chunks[2]; k++)
      {
         for (int j = 0; j < num_chunks[1]; j++)
         {
            get_chunk( buf,
                       num_chunks[0] - 1,
                       j,
                       k );

            int abs_chunk_x = map_pos_x / num_chunk_elements[0];
            int abs_chunk_y = map_pos_y / num_chunk_elements[1];
            int abs_chunk_z = map_pos_z / num_chunk_elements[2];

            std::string filename = create_filename( abs_chunk_x + num_chunks[0] - 1,
                                                    abs_chunk_y + j,
                                                    abs_chunk_z + k );

            queue->fill_buffer( filename,
                                buf,
                                0,
                                total_num_chunk_elements );
         }
      }

      // Shift the blocks forward modulo the chunk_dim_x
      for (int k = 0; k < dim_z; k++)
      {
         for (int j = 0; j < dim_y; j++)
         {
            for (int ind = 0, i = dim_x - chunk_dim_x; i < dim_x; i++, ind++)
            {
               buf[ind] = blocks[ i                 +
                                  j * dim_x         +
                                  k * dim_x * dim_y ];
            }
      
            for (int i = dim_x - 1; i >= chunk_dim_x; i--)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y ] =

                       blocks[ i                 +
                               j * dim_x         +
                               k * dim_x * dim_y -
                               chunk_dim_x        ];
            }

            for (int i = 0; i < chunk_dim_x; i++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y ] = buf[i];
            }
         }
      }

      // move the blocks, as a whole, appropriately
      map_pos_x -= (float)chunk_dim_x;
   }

   float displacement_y = position[1] - map_pos_y;

   if (displacement_y > chunk_dim_y)
   {
      for (int k = 0; k < dim_z; k++)
      {
         for (int ind = 0, j = 0; j < chunk_dim_y; j++)
         {
            for (int i = 0; i < dim_x; i++, ind++)
            {
               buf[ind] = blocks[ ind +
                                  k * dim_x * dim_y ];
            }
         }

         for (int j = 0; j < dim_y - chunk_dim_y; j++)
         {
            for (int i = 0; i < dim_x; i++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y ] =
                  blocks[ i                 +
                          j * dim_x         +
                          k * dim_x * dim_y +
                          chunk_dim_y * dim_x ];
            }
         }

         for (int ind = 0, j = 0; j < chunk_dim_y; j++)
         {
            for (int i = 0; i < dim_x; i++, ind++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y +
                       (dim_y - chunk_dim_y) * dim_x ] = buf[ind];
            }
         }

      }

      // move the blocks, as a whole, appropriately
      map_pos_y += (float)chunk_dim_y;
   }
   else if (displacement_y < 0.0f)
   {
      for (int k = 0; k < dim_z; k++)
      {
         for (int ind = 0, j = 0; j < chunk_dim_y; j++)
         {
            for (int i = 0; i < dim_x; i++, ind++)
            {
               buf[ind] = blocks[ i                 +
                                  j * dim_x         +
                                  k * dim_x * dim_y +
                                  (dim_y - chunk_dim_y) * dim_x ];
            }
         }

         for (int j = dim_y - 1; j >= chunk_dim_y; j--)
         {
            for (int i = 0; i < dim_x; i++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y ] =

                  blocks[ i                 +
                          j * dim_x         +
                          k * dim_x * dim_y -
                          chunk_dim_y * dim_x ];
            }
         }

         for (int ind = 0, j = 0; j < chunk_dim_y; j++)
         {
            for (int i = 0; i < dim_x; i++, ind++)
            {
               blocks[ i                 +
                       j * dim_x         +
                       k * dim_x * dim_y ] = buf[ind];
            }
         }
      }
      // move the blocks, as a whole, appropriately
      map_pos_y -= (float)chunk_dim_y;
   }

   float displacement_z = position[2] - map_pos_z;

   if (displacement_z > chunk_dim_z)
   {
      int dim_xy = dim_x * dim_y;

      for (int k = 0, ind = 0; k < chunk_dim_z; k++)
      {
         for ( int ij = 0; ij < dim_xy; ij++, ind++)
         {
            buf[ind] = blocks[ind];
         }
      }

      for (int k = 0; k < dim_z - chunk_dim_z; k++)
      {
         for ( int ij = 0; ij < dim_xy; ij++)
         {
            blocks[ k * dim_xy + ij ] =
               blocks[ (k + chunk_dim_z) * dim_xy + ij ];
         }
      }

      for (int k = dim_z - chunk_dim_z, ind = 0; k < dim_z; k++)
      {
         for ( int ij = 0; ij < dim_xy; ij++, ind++)
         {
            blocks[ k * dim_xy + ij ] =
               buf[ind];
         }
      }

      map_pos_z += (float)chunk_dim_z;
   }
   else if (displacement_z < 0.0f)
   {
      int dim_xy = dim_x * dim_y;

      for (int k = dim_z - chunk_dim_z, ind = 0; k < dim_z; k++)
      {
         for (int ij = 0; ij < dim_xy; ij++, ind++)
         {
            buf[ind] = blocks[ k * dim_xy + ij ];
         }
      }

      for (int k = dim_z - 1; k >= chunk_dim_z; k--)
      {
         for (int ij = 0; ij < dim_xy; ij++)
         {
            blocks[ k * dim_xy + ij ] =
               blocks[ (k - chunk_dim_z) * dim_xy + ij ];
         }
      }

      for (int k = 0, ind = 0; k < chunk_dim_z; k++)
      {
         for (int ij = 0; ij < dim_xy; ij++, ind++)
         {
            blocks[ k * dim_xy + ij ] = buf[ind];
         }
      }

      map_pos_z -= (float)chunk_dim_z;
   }

#endif

   /*
   ** find which chunk (virtual location) the position is at
   */
   for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
   {
      for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
      {
         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {

            Chunk *this_chunk = access_chunk( x_dir,
                                              y_dir,
                                              z_dir );

            if (this_chunk->position_in_chunk( position ))
            {
               virtual_grid[0] = virtual_chunk_id_x[x_dir];
               virtual_grid[1] = virtual_chunk_id_y[y_dir];
               virtual_grid[2] = virtual_chunk_id_z[z_dir];
            }

         }
      }
   }

   /*
   ** shift the chunks to align the user position
   ** with the middle chunk
   */
   int mid_block[3] = { num_chunks[0] / 2,
                        num_chunks[1] / 2,
                        num_chunks[2] / 2 };

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

/*
** function name: shift from: Map
** Circular shift the cube in the specified direction
*/
void Map::shift( int x, int y, int z)
{

   /*
   ** Update grid mapping in first dimension
   */
   for (int k = 0; k < num_chunks[0]; k++)
   {
      
      virtual_chunk_id_x[k] -= x;
      if (virtual_chunk_id_x[k] >= num_chunks[0])
      {
         for (;virtual_chunk_id_x[k] >= num_chunks[0];
               virtual_chunk_id_x[k] -= num_chunks[0]) { /* NA */ }

         for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( k,
                                                 y_dir,
                                                 z_dir);

               this_chunk->move( -num_chunks[0], 0, 0, queue);
            }
         }

      }

      if (virtual_chunk_id_x[k] < 0)
      {
         for (;virtual_chunk_id_x[k] < 0; virtual_chunk_id_x[k] += num_chunks[0]) { /* NA */ }

         for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( k,
                                                 y_dir,
                                                 z_dir);

               this_chunk->move( num_chunks[0], 0, 0, queue);
            }
         }
      }

      /*
      ** Update the virtual-to-physical grid mapping
      */
      physical_chunk_id_x[virtual_chunk_id_x[k]] = k;
   }

   for (int k = 0; k < num_chunks[1]; k++)
   {
      virtual_chunk_id_y[k] -= y;
      if (virtual_chunk_id_y[k] >= num_chunks[1])
      {
         for (;virtual_chunk_id_y[k] >= num_chunks[1];
               virtual_chunk_id_y[k] -= num_chunks[1]) { /* NA */ }

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 k,
                                                 z_dir);

               this_chunk->move( 0, -num_chunks[1], 0, queue);
            }
         }

      }

      if (virtual_chunk_id_y[k] < 0)
      {
         for (;virtual_chunk_id_y[k] < 0; virtual_chunk_id_y[k] += num_chunks[1]) { /* NA */ }

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 k,
                                                 z_dir);

               this_chunk->move( 0, num_chunks[1], 0, queue);
            }
         }
      }

      /*
      ** Update the virtual-to-physical grid mapping
      */
      physical_chunk_id_y[virtual_chunk_id_y[k]] = k;

   }

   for (int k = 0; k < num_chunks[2]; k++)
   {
      virtual_chunk_id_z[k] -= z;
      if (virtual_chunk_id_z[k] >= num_chunks[2])
      {
         for (;virtual_chunk_id_z[k] >= num_chunks[2];
               virtual_chunk_id_z[k] -= num_chunks[2]) { /* NA */ }

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 y_dir,
                                                 k);

               this_chunk->move( 0, 0, -num_chunks[2], queue);
            }
         }

      }

      if (virtual_chunk_id_z[k] < 0)
      {
         for (;virtual_chunk_id_z[k] < 0; virtual_chunk_id_z[k] += num_chunks[2]) { /* NA */ }

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 y_dir,
                                                 k );

               this_chunk->move( 0, 0, num_chunks[2], queue);
            }
         }
      }

      /*
      ** Update the virtual-to-physical grid mapping
      */
      physical_chunk_id_z[virtual_chunk_id_z[k]] = k;
   }

   // update the queue if any movement has happened
   for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
   {
      for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
      {
         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {

#ifndef BLOCKS
            Chunk *this_chunk = access_chunk( x_dir,
                                              y_dir,
                                              z_dir );

            this_chunk->populate( queue );
#endif

         }
      }
   }
}

/*
** function name: move from Chunk 
**
** move the chunk a discrete number of chunks
*/
void Chunk::move( int x, int y, int z, Queue* queue )
{

   prev_abs_pos_id[0] = abs_pos_id[0];
   prev_abs_pos_id[1] = abs_pos_id[1];
   prev_abs_pos_id[2] = abs_pos_id[2];

   abs_pos_id[0] += x;
   abs_pos_id[1] += y;
   abs_pos_id[2] += z;

   position[0] += static_cast<float>(x) * chunk_dim[0];
   position[1] += static_cast<float>(y) * chunk_dim[1];
   position[2] += static_cast<float>(z) * chunk_dim[2];

   valid      = false;
   reassigned = true;
}

