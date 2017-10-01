#include "map.h"
#include "queue.h"
#include "read_chunk.h"

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

   Workspace l_workspace = workspace;
   int* buffer = l_workspace.reserve<int>( 1024 );

   bool disp_forward  = false;
   bool disp_backward = false;
   bool disp_left     = false;
   bool disp_right    = false;
   bool disp_up       = false;
   bool disp_down     = false;

   int total_num_chunk_elements = chunk_dim_x *
                                  chunk_dim_y *
                                  chunk_dim_z;

   float displacement_x = position[0] - map_pos_x;

   if (displacement_x > chunk_dim_x * 1.5f)
   {

      disp_forward = true;

      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int j = 0; j < num_chunks_y; j++)
         {

            int ind = j * num_chunks_x +
                      k * num_chunks_x * num_chunks_y;

            if (write_permissions[ind] == true)
            {
               get_chunk (
                     buffer,
                     0,
                     j,
                     k,
                     0 );

               std::string filename =
                  create_filename (
                        abs_chunk_x + 0,
                        abs_chunk_y + j,
                        abs_chunk_z + k );

               queue->fill_buffer (
                     filename,
                     buffer,
                     0,
                     total_num_chunk_elements * sizeof(*buffer) );
            }

            // circularly shift the write permissions cube backward (the other way)
            // for this (:,j,k) th row

            int sh_ind;
            for (sh_ind = 0; sh_ind < num_chunks_x - 1; sh_ind++)
            {
               write_permissions[ind + sh_ind] = write_permissions[ind + sh_ind + 1];
            }
         }
      }

      // Shift the blocks back modulo the chunk_dim_x
      for (int chunk_z = 0; chunk_z < num_chunks_z; chunk_z++)
      {
         for (int chunk_y = 0; chunk_y < num_chunks_y; chunk_y++)
         {
            for (int chunk_x = 0; chunk_x < num_chunks_x - 1; chunk_x++)
            {
               get_chunk(
                     buffer,
                     chunk_x + 1,
                     chunk_y,
                     chunk_z,
                     0 );

               set_chunk(
                     buffer,
                     chunk_x,
                     chunk_y,
                     chunk_z );
            }

            int chunk_dim_xyz = chunk_dim_x * chunk_dim_y * chunk_dim_z;
            for (int ind = 0; ind < chunk_dim_xyz; ind++) buffer[ind] = 0.0f;

            set_chunk(
                  buffer,
                  num_chunks_x - 1,
                  chunk_y,
                  chunk_z );
         }
      }
           
      // move the blocks, as a whole, appropriately
      map_pos_x += (float)chunk_dim_x;
   }

   else if (displacement_x < -chunk_dim_x * 0.5f)
   {

      disp_backward = true;

      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int j = 0; j < num_chunks_y; j++)
         {
            int ind = num_chunks_x - 1 +
                      j * num_chunks_x +
                      k * num_chunks_x * num_chunks_y;

            if (write_permissions[ind] == true)
            {
               get_chunk(
                     buffer,
                     num_chunks_x - 1,
                     j,
                     k,
                     0 );

               std::string filename = create_filename(
                     abs_chunk_x + num_chunks_x - 1,
                     abs_chunk_y + j,
                     abs_chunk_z + k );

               queue->fill_buffer(
                     filename,
                     buffer,
                     0,
                     total_num_chunk_elements * sizeof(*buffer) );
            }

            // circularly shift the write permissions cube forward (the other way)
            // for this (:,j,k) th row

            int sh_ind;
            for (sh_ind = 0; sh_ind < num_chunks_x - 1; sh_ind++)
            {
               write_permissions[ind - sh_ind] = write_permissions[ind - sh_ind - 1];
            }
         }
      }

      // Shift the blocks forward modulo the chunk_dim_x
      for (int chunk_z = 0; chunk_z < num_chunks_z; chunk_z++)
      {
         for (int chunk_y = 0; chunk_y < num_chunks_y; chunk_y++)
         {
            for (int chunk_x = num_chunks_x - 1; chunk_x > 0; chunk_x--)
            {
               get_chunk(
                     buffer,
                     chunk_x - 1,
                     chunk_y,
                     chunk_z,
                     0 );

               set_chunk(
                     buffer,
                     chunk_x,
                     chunk_y,
                     chunk_z );
            }

            int chunk_dim_xyz = chunk_dim_x * chunk_dim_y * chunk_dim_z;
            for (int ind = 0; ind < chunk_dim_xyz; ind++) buffer[ind] = 0.0f;

            set_chunk(
                  buffer,
                  0,
                  chunk_y,
                  chunk_z );
         }
      }

      // move the blocks, as a whole, appropriately
      map_pos_x -= (float)chunk_dim_x;
   }

   float displacement_y = position[1] - map_pos_y;

   if (displacement_y > chunk_dim_y * 1.5f)
   {

      disp_left = true;

      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int i = 0; i < num_chunks_x; i++)
         {
            if ( (disp_forward  == false || i < num_chunks_x - 1) &&
                  (disp_backward == false || i > 0) )
            {
               int ind = i +
                         k * num_chunks_x * num_chunks_y;

               if (write_permissions[ind] == true)
               {
                  get_chunk(
                        buffer,
                        i,
                        0,
                        k,
                        0 );

                  std::string filename = create_filename(
                        abs_chunk_x + i,
                        abs_chunk_y + 0,
                        abs_chunk_z + k );

                  queue->fill_buffer(
                        filename,
                        buffer,
                        0,
                        total_num_chunk_elements * sizeof(*buffer) );
               }

               // circularly shift the write permissions cube to the right (the other way)
               // for this (i,:,k) th row

               int sh_ind;
               for (sh_ind = 0; sh_ind < num_chunks_y - 1; sh_ind++)
               {
                  write_permissions[ind + sh_ind * num_chunks_x] =
                     write_permissions[ind + (sh_ind + 1) * num_chunks_x];
               }
            }
         }
      }

      for (int k = 0; k < dim_z; k++)
      {
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
      }

      // move the blocks, as a whole, appropriately
      map_pos_y += (float)chunk_dim_y;
   }

   else if (displacement_y < -chunk_dim_y * 0.5f)
   {

      disp_right = true;

      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int i = 0; i < num_chunks_x; i++)
         {
            if ( (disp_forward  == false || i < num_chunks_x - 1) &&
                  (disp_backward == false || i > 0) )
            {
               int ind = i +
                         (num_chunks_y - 1) * num_chunks_x +
                         k * num_chunks_x * num_chunks_y;

               if (write_permissions[ind] == true)
               {
                  get_chunk(
                        buffer,
                        i,
                        num_chunks_y - 1,
                        k,
                        0 );

                  std::string filename = create_filename(
                        abs_chunk_x + i,
                        abs_chunk_y + num_chunks_y - 1,
                        abs_chunk_z + k );

                  queue->fill_buffer(
                        filename,
                        buffer,
                        0,
                        total_num_chunk_elements * sizeof(*buffer) );
               }

               // circularly shift the write permissions cube to the right (the other way)
               // for this (i,:,k) th row

               int sh_ind;
               for (sh_ind = 0; sh_ind < num_chunks_y - 1; sh_ind++)
               {
                  write_permissions[ind - sh_ind * num_chunks_x] =
                     write_permissions[ind - (sh_ind + 1) * num_chunks_x];
               }
            }
         }
      }

      for (int k = 0; k < dim_z; k++)
      {
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
      }
      // move the blocks, as a whole, appropriately
      map_pos_y -= (float)chunk_dim_y;
   }

   float displacement_z = position[2] - map_pos_z;

   if (displacement_z > chunk_dim_z * 1.5f)
   {

      disp_up = true;

      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int i = 0; i < num_chunks_x; i++)
      {
         for (int j = 0; j < num_chunks_y; j++)
         {
            if ( (disp_forward  == false || i < num_chunks_x - 1) &&
                 (disp_backward == false || i > 0) &&
                 (disp_left     == false || j < num_chunks_y - 1) &&
                 (disp_right    == false || j > 0) )
            {
               int ind = i +
                  j * num_chunks_x;

               if (write_permissions[ind] == true)
               {
                  get_chunk(
                        buffer,
                        i,
                        j,
                        0,
                        0 );

                  std::string filename = create_filename(
                        abs_chunk_x + i,
                        abs_chunk_y + j,
                        abs_chunk_z + 0 );

                  queue->fill_buffer(
                        filename,
                        buffer,
                        0,
                        total_num_chunk_elements * sizeof(*buffer) );
               }
               // circularly shift the write permissions cube to the up
               // for this (i,j,:) th row

               int sh_ind;
               for (sh_ind = 0; sh_ind < num_chunks_z - 1; sh_ind++)
               {
                  write_permissions[ind + sh_ind * num_chunks_x * num_chunks_y] =
                     write_permissions[ind + (sh_ind + 1) * num_chunks_x * num_chunks_y];
               }
            }
         }
      }

      int dim_xy = dim_x * dim_y;

      for (int k = 0; k < dim_z - chunk_dim_z; k++)
      {
         for ( int ij = 0; ij < dim_xy; ij++)
         {
            blocks[ k * dim_xy + ij ] =
               blocks[ (k + chunk_dim_z) * dim_xy + ij ];
         }
      }

      map_pos_z += (float)chunk_dim_z;
   }

   else if (displacement_z < -chunk_dim_z * 0.5f)
   {

      disp_down = true;

      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int i = 0; i < num_chunks_x; i++)
      {
         for (int j = 0; j < num_chunks_y; j++)
         {
            if ( (disp_forward  == false || i < num_chunks_x - 1) &&
                 (disp_backward == false || i > 0) &&
                 (disp_left     == false || j < num_chunks_y - 1) &&
                 (disp_right    == false || j > 0) )
            {
               int ind = i +
                  j * num_chunks_x +
                  (num_chunks_z - 1) * num_chunks_x * num_chunks_y;

               if (write_permissions[ind] == true)
               {
                  get_chunk(
                        buffer,
                        i,
                        j,
                        num_chunks_z - 1,
                        0 );

                  std::string filename = create_filename(
                        abs_chunk_x + i,
                        abs_chunk_y + j,
                        abs_chunk_z + num_chunks_z - 1 );

                  queue->fill_buffer(
                        filename,
                        buffer,
                        0,
                        total_num_chunk_elements * sizeof(*buffer) );
               }
               // circularly shift the write permissions cube down
               // for this (i,j,:) th row

               int sh_ind;
               for (sh_ind = 0; sh_ind < num_chunks_z - 1; sh_ind++)
               {
                  write_permissions[ind - sh_ind * num_chunks_x * num_chunks_y] =
                     write_permissions[ind + (sh_ind - 1) * num_chunks_x * num_chunks_y];
               }
            }
         }
      }

      int dim_xy = dim_x * dim_y;

      for (int k = dim_z - 1; k >= chunk_dim_z; k--)
      {
         for (int ij = 0; ij < dim_xy; ij++)
         {
            blocks[ k * dim_xy + ij ] =
               blocks[ (k - chunk_dim_z) * dim_xy + ij ];
         }
      }

      map_pos_z -= (float)chunk_dim_z;
   }

   /**************************************************************************
   ** identify files to be read in
   ***************************************************************************/

   if (disp_forward)
   {
      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int j = 0; j < num_chunks_y; j++)
         {
            int abs_chunk[3] = { abs_chunk_x + num_chunks_x - 1,
                                 abs_chunk_y + j,
                                 abs_chunk_z + k };
            readQueue.new_node( abs_chunk, 3 );
         }
      }
   }
   else if (disp_backward)
   {
      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int j = 0; j < num_chunks_y; j++)
         {
            int abs_chunk[3] = { abs_chunk_x,
                                 abs_chunk_y + j,
                                 abs_chunk_z + k };
            readQueue.new_node( abs_chunk, 3 );
         }
      }
   }
   if (disp_left)
   {
      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int i = 0; i < num_chunks_x; i++)
         {
            int abs_chunk[3] = { abs_chunk_x + i,
                                 abs_chunk_y + num_chunks_y - 1,
                                 abs_chunk_z + k };
            readQueue.new_node( abs_chunk, 3 );
         }
      }
   }
   else if (disp_right)
   {
      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int k = 0; k < num_chunks_z; k++)
      {
         for (int i = 0; i < num_chunks_x; i++)
         {
            int abs_chunk[3] = { abs_chunk_x + i,
                                 abs_chunk_y,
                                 abs_chunk_z + k };
            readQueue.new_node( abs_chunk, 3 );
         }
      }
   }
   if (disp_up)
   {
      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int j = 0; j < num_chunks_y; j++)
      {
         for (int i = 0; i < num_chunks_x; i++)
         {
            int abs_chunk[3] = { abs_chunk_x + i,
                                 abs_chunk_y + j,
                                 abs_chunk_z + num_chunks_z - 1 };
            readQueue.new_node( abs_chunk, 3 );
         }
      }
   }
   else if (disp_down)
   {
      int abs_chunk_x = map_pos_x / chunk_dim_x;
      int abs_chunk_y = map_pos_y / chunk_dim_y;
      int abs_chunk_z = map_pos_z / chunk_dim_z;

      for (int j = 0; j < num_chunks_y; j++)
      {
         for (int i = 0; i < num_chunks_x; i++)
         {
            int abs_chunk[3] = { abs_chunk_x + i,
                                 abs_chunk_y + j,
                                 abs_chunk_z };
            readQueue.new_node( abs_chunk, 3 );
         }
      }
   }

   // wait for IO thread to finish writing chunks to disk
   if ( disp_forward || disp_backward || disp_left || disp_right ||
        disp_up      || disp_down )
   {
      pthread_barrier_wait( update_barrier );
   }

}
