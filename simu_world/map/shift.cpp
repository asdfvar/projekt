#include "map.h"
#include "queue.h"

#define DO_QUEUE

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
   bool disp_forward  = false;
   bool disp_backward = false;
   bool disp_left     = false;
   bool disp_right    = false;

   int total_num_chunk_elements = num_chunk_elements[0] *
                                  num_chunk_elements[1] *
                                  num_chunk_elements[2];

   float displacement_x = position[0] - map_pos_x;

   if (displacement_x > chunk_dim_x * 1.5f)
   {

      disp_forward = true;

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

#ifdef DO_QUEUE
            queue->fill_buffer( filename,
                                buf,
                                0,
                                total_num_chunk_elements );
#endif
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

   else if (displacement_x < -chunk_dim_x * 0.5f)
   {

      disp_backward = true;

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

#ifdef DO_QUEUE
            queue->fill_buffer( filename,
                                buf,
                                0,
                                total_num_chunk_elements );
#endif
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

   if (displacement_y > chunk_dim_y * 1.5f)
   {

      disp_left = true;

      for (int k = 0; k < num_chunks[2]; k++)
      {
         for (int i = 0; i < num_chunks[0]; i++)
         {
            if ( (disp_forward  == false || i < num_chunks[0] - 1) &&
                 (disp_backward == false || i > 0) )
            {
               get_chunk( buf,
                          i,
                          0,
                          k );

               int abs_chunk_x = map_pos_x / num_chunk_elements[0];
               int abs_chunk_y = map_pos_y / num_chunk_elements[1];
               int abs_chunk_z = map_pos_z / num_chunk_elements[2];

               std::string filename = create_filename( abs_chunk_x + i,
                                                       abs_chunk_y + 0,
                                                       abs_chunk_z + k );

#ifdef DO_QUEUE
               queue->fill_buffer( filename,
                                   buf,
                                   0,
                                   total_num_chunk_elements );
#endif
            }
         }
      }

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

   else if (displacement_y < -chunk_dim_y * 0.5f)
   {

      disp_right = true;

      for (int k = 0; k < num_chunks[2]; k++)
      {
         for (int i = 0; i < num_chunks[0]; i++)
         {
            if ( (disp_forward  == false || i < num_chunks[0] - 1) &&
                 (disp_backward == false || i > 0) )
            {
               get_chunk( buf,
                          i,
                          num_chunks[1] - 1,
                          k );

               int abs_chunk_x = map_pos_x / num_chunk_elements[0];
               int abs_chunk_y = map_pos_y / num_chunk_elements[1];
               int abs_chunk_z = map_pos_z / num_chunk_elements[2];

               std::string filename = create_filename( abs_chunk_x + i,
                                                       abs_chunk_y + num_chunks[1] - 1,
                                                       abs_chunk_z + k );

#ifdef DO_QUEUE
               queue->fill_buffer( filename,
                                   buf,
                                   0,
                                   total_num_chunk_elements );
#endif
            }
         }
      }

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

   if (displacement_z > chunk_dim_z * 1.5f)
   {

      for (int i = 0; i < num_chunks[0]; i++)
      {
         for (int j = 0; j < num_chunks[1]; j++)
         {
            if ( (disp_forward  == false || i < num_chunks[0] - 1) &&
                 (disp_backward == false || i > 0) &&
                 (disp_left     == false || j < num_chunks[1] - 1) &&
                 (disp_right    == false || j > 0) )
            {
               get_chunk( buf,
                          i,
                          j,
                          0 );

               int abs_chunk_x = map_pos_x / num_chunk_elements[0];
               int abs_chunk_y = map_pos_y / num_chunk_elements[1];
               int abs_chunk_z = map_pos_z / num_chunk_elements[2];

               std::string filename = create_filename( abs_chunk_x + i,
                                                       abs_chunk_y + j,
                                                       abs_chunk_z + 0 );

#ifdef DO_QUEUE
               queue->fill_buffer( filename,
                                   buf,
                                   0,
                                   total_num_chunk_elements );
#endif
            }
         }
      }

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

   else if (displacement_z < -chunk_dim_z * 0.5f)
   {

      for (int i = 0; i < num_chunks[0]; i++)
      {
         for (int j = 0; j < num_chunks[1]; j++)
         {
            if ( (disp_forward  == false || i < num_chunks[0] - 1) &&
                 (disp_backward == false || i > 0) &&
                 (disp_left     == false || j < num_chunks[1] - 1) &&
                 (disp_right    == false || j > 0) )
            {
               get_chunk( buf,
                          i,
                          j,
                          num_chunks[2] - 1 );

               int abs_chunk_x = map_pos_x / num_chunk_elements[0];
               int abs_chunk_y = map_pos_y / num_chunk_elements[1];
               int abs_chunk_z = map_pos_z / num_chunk_elements[2];

               std::string filename = create_filename( abs_chunk_x + i,
                                                       abs_chunk_y + j,
                                                       abs_chunk_z + num_chunks[2] - 1 );

#ifdef DO_QUEUE
               queue->fill_buffer( filename,
                                   buf,
                                   0,
                                   total_num_chunk_elements );
#endif
            }
         }
      }
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

   if ( disp_forward || disp_backward || disp_left || disp_right )
   {
      pthread_barrier_wait( update_barrier );
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
      }

      if (virtual_chunk_id_x[k] < 0)
      {
         for (;virtual_chunk_id_x[k] < 0; virtual_chunk_id_x[k] += num_chunks[0]) { /* NA */ }
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
      }

      if (virtual_chunk_id_y[k] < 0)
      {
         for (;virtual_chunk_id_y[k] < 0; virtual_chunk_id_y[k] += num_chunks[1]) { /* NA */ }
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
      }

      if (virtual_chunk_id_z[k] < 0)
      {
         for (;virtual_chunk_id_z[k] < 0; virtual_chunk_id_z[k] += num_chunks[2]) { /* NA */ }
      }

      /*
      ** Update the virtual-to-physical grid mapping
      */
      physical_chunk_id_z[virtual_chunk_id_z[k]] = k;
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

