#include "map.h"
#include <iostream>
#include <cstdlib>
#include "draw_block.h"
#include "fileio.h"
#include <cmath>

/*
** constructor name: Map
*/
Map::Map( pthread_barrier_t* IO_barrier_in )
{

   IO_barrier = IO_barrier_in;

   num_chunks[0] = 13;
   num_chunks[1] = 13;
   num_chunks[2] = 13;
   int total_local_grid_size = num_chunks[0] * num_chunks[1] * num_chunks[2];

   chunks_new = new Chunks_new( num_chunks );

   /*
   ** Initialize the mapping from the virtual grid index
   ** to the physical grid index
   */
   physical_chunk_id_x = new int[num_chunks[0]];
   physical_chunk_id_y = new int[num_chunks[1]];
   physical_chunk_id_z = new int[num_chunks[2]];

   for (int k = 0; k < num_chunks[0]; k++) physical_chunk_id_x[k] = k;
   for (int k = 0; k < num_chunks[1]; k++) physical_chunk_id_y[k] = k;
   for (int k = 0; k < num_chunks[2]; k++) physical_chunk_id_z[k] = k;

   /*
   ** Initialize the mapping from the physical grid
   ** to the virtual grid index
   */
   virtual_chunk_id_x = new int[num_chunks[0]];
   virtual_chunk_id_y = new int[num_chunks[1]];
   virtual_chunk_id_z = new int[num_chunks[2]];

   for (int k = 0; k < num_chunks[0]; k++) virtual_chunk_id_x[k] = k;
   for (int k = 0; k < num_chunks[1]; k++) virtual_chunk_id_y[k] = k;
   for (int k = 0; k < num_chunks[2]; k++) virtual_chunk_id_z[k] = k;

   /*
   ** Create the save directory
   */
   fio::directory();

   num_chunk_elements[0] = 5;
   num_chunk_elements[1] = 5;
   num_chunk_elements[2] = 5;
   int ind;
   int abs_pos_id[3];

   for (int k = 0, ind_z = -num_chunks[2]/2, ind = 0; ind_z <= num_chunks[2]/2; ind_z++, k++)
   {
      for (int j = 0, ind_y = -num_chunks[1]/2; ind_y <= num_chunks[1]/2; ind_y++, j++)
      {
         for (int i = 0, ind_x = -num_chunks[0]/2; ind_x <= num_chunks[0]/2; ind_x++, ind++, i++)
         {
           float position[3] = { (float)(ind_x * num_chunk_elements[0]),
                                 (float)(ind_y * num_chunk_elements[1]),
                                 (float)(ind_z * num_chunk_elements[2]) };

           abs_pos_id[0] = ind_x;
           abs_pos_id[1] = ind_y;
           abs_pos_id[2] = ind_z;
           /*
           ** Create the chunks for this map
           */
           chunks_new->insert_chunk( new Chunk(ind, abs_pos_id, num_chunk_elements, position) );

         }
      }
   }

   chunks_new->set_base();

   // wait for the IO thread to have finished its initialization before continuing
   //pthread_barrier_wait( IO_barrier );

   std::cout << __FILE__ << ":" << __LINE__ << ":finished creating map" << std::endl;

}

/*
** destructor name: ~Map
*/
Map::~Map(void)
{
   delete chunks_new;

   delete[] physical_chunk_id_x;
   delete[] physical_chunk_id_y;
   delete[] physical_chunk_id_z;

   delete[] virtual_chunk_id_x;
   delete[] virtual_chunk_id_y;
   delete[] virtual_chunk_id_z;

}

/*
** function name: update from: Map
** Update chunks from the IO thread
*/
void Map::update( void )
{

   for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
   {
      for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
      {
         for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
         {

            Chunk *this_chunk = access_chunk( x_dir,
                                              y_dir,
                                              z_dir );
            this_chunk->update();
         }
      }
   }

}

/*
** function name: update from: Map
**
** update map information from user position.
** when the user is no longer at the center of
** the map, shift the chunks until the user is
** at the center of the map
*/
void Map::update( float *position)
{
   int virtual_grid[3] = { -1, -1, -1 };

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
** function name: render_chunk from: Map
*/
void Map::render_chunk( User *user)
{
   float block_position[3];
   Chunk *chunk;

   int mid_chunk[3] = { num_chunks[0] / 2,
                        num_chunks[1] / 2,
                        num_chunks[2] / 2 };

   float user_position[3];
   user->get_position( user_position );

   float user_direction[3];
   user->get_direction( user_direction );

   float window_distance = user->get_window_distance();
   float window_width    = user->get_window_width();

   for (int chunk_ind_z = 0; chunk_ind_z < num_chunks[2]; chunk_ind_z++)
   {
      for (int chunk_ind_y = 0; chunk_ind_y < num_chunks[1]; chunk_ind_y++)
      {
         for (int chunk_ind_x = 0; chunk_ind_x < num_chunks[0]; chunk_ind_x++)
         {
            chunk = access_chunk( chunk_ind_x, chunk_ind_y, chunk_ind_z);

            int chunk_sides = 0;

            if ( virtual_chunk_id_x[ chunk_ind_x ] >= mid_chunk[0] )
            {
               chunk_sides |= DRAW_BACK;
            }
            if ( virtual_chunk_id_x[ chunk_ind_x ] <= mid_chunk[0] )
            {
               chunk_sides |= DRAW_FRONT;
            }
            if ( virtual_chunk_id_y[ chunk_ind_y ] >= mid_chunk[1] )
            {
               chunk_sides |= DRAW_RIGHT;
            }
            if ( virtual_chunk_id_y[ chunk_ind_y ] <= mid_chunk[1] )
            {
               chunk_sides |= DRAW_LEFT;
            }
            if ( virtual_chunk_id_z[ chunk_ind_z ] >= mid_chunk[2] )
            {
               chunk_sides |= DRAW_BOTTOM;
            }
            if ( virtual_chunk_id_z[ chunk_ind_z ] <= mid_chunk[2] )
            {
               chunk_sides |= DRAW_TOP;
            }

            if ( chunk->is_valid() )
            {
               // iterate through all the blocks
               for (unsigned int block_ind = 0; block_ind < chunk->get_dimensions(); block_ind++)
               {

                  int block_sides = 0;
                  int i_block_position[3];
                  bool front_open, back_open, left_open, right_open, top_open, bottom_open;
                  front_open = back_open = left_open =
                  right_open = top_open = bottom_open = true;

                  int block = chunk->get_position( block_position, block_ind );
                  if ( block == 0 ) continue;

                  int front_block_ind = block_ind + 1;
                  if( block_ind % num_chunk_elements[0] + 1 < num_chunk_elements[0] )
                  {
                     int front_block = chunk->get_position( NULL, front_block_ind );
                     if (front_block != 0) front_open = false;
                  }

                  int back_block_ind = block_ind - 1;
                  if( block_ind % num_chunk_elements[0] > 0 )
                  {
                     int back_block = chunk->get_position( NULL, back_block_ind );
                     if (back_block != 0) back_open = false;
                  }

                  int left_block_ind = block_ind + num_chunk_elements[0];
                  if( (block_ind % (num_chunk_elements[0] * num_chunk_elements[1]))/num_chunk_elements[0] + 1 < num_chunk_elements[1])
                  {
                     int left_block = chunk->get_position( NULL, left_block_ind );
                     if (left_block != 0) left_open = false;
                  }

                  int right_block_ind = block_ind - num_chunk_elements[0];
                  if( (block_ind % (num_chunk_elements[0] * num_chunk_elements[1]))/num_chunk_elements[0] > 0)
                  {
                     int right_block = chunk->get_position( NULL, right_block_ind );
                     if (right_block != 0) right_open = false;
                  }

                  int top_block_ind = block_ind + num_chunk_elements[0] * num_chunk_elements[1];
                  if( block_ind / (num_chunk_elements[0] * num_chunk_elements[1]) + 1 < num_chunk_elements[2] )
                  {
                     int top_block = chunk->get_position( NULL, top_block_ind );
                     if (top_block != 0) top_open = false;
                  }

                  int bottom_block_ind = block_ind - num_chunk_elements[0] * num_chunk_elements[1];
                  if( block_ind / (num_chunk_elements[0] * num_chunk_elements[1]) > 0)
                  {
                     int bottom_block = chunk->get_position( NULL, bottom_block_ind );
                     if (bottom_block != 0) bottom_open = false;
                  }

#if 0
                  i_block_position[0] = floorf( block_position[0] );
                  i_block_position[1] = floorf( block_position[1] );
                  i_block_position[2] = floorf( block_position[2] );

                  int near_block_position[3];

                  near_block_position[0] = i_block_position[0] + 1;
                  near_block_position[1] = i_block_position[1];
                  near_block_position[2] = i_block_position[2];

                  front_open = get_abs_element( near_block_position ) == 0;

                  near_block_position[0] = i_block_position[0] - 1;
                  near_block_position[1] = i_block_position[1];
                  near_block_position[2] = i_block_position[2];

                  back_open = get_abs_element( near_block_position ) == 0;

                  near_block_position[0] = i_block_position[0];
                  near_block_position[1] = i_block_position[1] + 1;
                  near_block_position[2] = i_block_position[2];

                  left_open = get_abs_element( near_block_position ) == 0;

                  near_block_position[0] = i_block_position[0];
                  near_block_position[1] = i_block_position[1] - 1;
                  near_block_position[2] = i_block_position[2];

                  right_open = get_abs_element( near_block_position ) == 0;

                  near_block_position[0] = i_block_position[0];
                  near_block_position[1] = i_block_position[1];
                  near_block_position[2] = i_block_position[2] + 1;

                  top_open = get_abs_element( near_block_position ) == 0;

                  near_block_position[0] = i_block_position[0];
                  near_block_position[1] = i_block_position[1];
                  near_block_position[2] = i_block_position[2] - 1;

                  bottom_open = get_abs_element( near_block_position ) == 0;
#endif

                  if ( front_open  ) block_sides |= DRAW_FRONT;
                  if ( back_open   ) block_sides |= DRAW_BACK;
                  if ( right_open  ) block_sides |= DRAW_RIGHT;
                  if ( left_open   ) block_sides |= DRAW_LEFT;
                  if ( top_open    ) block_sides |= DRAW_TOP;
                  if ( bottom_open ) block_sides |= DRAW_BOTTOM;

                  float color[3];
                  chunk->get_color( color );

                  int sides = chunk_sides & block_sides;

                  if ( sides )
                  {
                     draw_block( block_position,
                                 user_position,
                                 user_direction,
                                 window_distance,
                                 window_width,
                                 sides,
                                 color );
                  }
               }
            }
         }
      }
   }
}

/*
** function name: access_chunk from: Map
** Return pointer to the specified chunk block from physical id input
*/
Chunk *Map::access_chunk( int p_id_x,
                          int p_id_y,
                          int p_id_z )
{
   if ((p_id_x < 0) || (p_id_x >= num_chunks[0]) ||
       (p_id_y < 0) || (p_id_y >= num_chunks[1]) ||
       (p_id_z < 0) || (p_id_z >= num_chunks[2]))
   {
      std::cout << "select index of " << p_id_x << ", "  << p_id_y << ", "  << p_id_z <<
                   " out of bounds: " << num_chunks[0] << ", " <<
                                         num_chunks[1] << ", " <<
                                         num_chunks[2] << std::endl;
   }

   unsigned int index =  p_id_z * num_chunks[0] * num_chunks[1] +
                         p_id_y * num_chunks[0] +
                         p_id_x;

   return chunks_new->at( index );

}

void Map::set_phys_chunk_color( int    p_ind_x,
                                int    p_ind_y,
                                int    p_ind_z,
                                float *color )
{
   Chunk *chunk = access_chunk( p_ind_x,
                                p_ind_y,
                                p_ind_z );

   chunk->set_color( color );
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
               virtual_chunk_id_x[k] -= num_chunks[0]) { /* NA */}

         for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( k,
                                                 y_dir,
                                                 z_dir);

               this_chunk->move( -num_chunks[0], 0, 0);
            }
         }

      }

      if (virtual_chunk_id_x[k] < 0)
      {
         for (;virtual_chunk_id_x[k] < 0; virtual_chunk_id_x[k] += num_chunks[0]) { /* NA */}

         for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( k,
                                                 y_dir,
                                                 z_dir);

               this_chunk->move( num_chunks[0], 0, 0);
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
               virtual_chunk_id_y[k] -= num_chunks[1]) { /* NA */}

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 k,
                                                 z_dir);

               this_chunk->move( 0, -num_chunks[1], 0);
            }
         }

      }

      if (virtual_chunk_id_y[k] < 0)
      {
         for (;virtual_chunk_id_y[k] < 0; virtual_chunk_id_y[k] += num_chunks[1]) { /* NA */}

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 k,
                                                 z_dir);

               this_chunk->move( 0, num_chunks[1], 0);
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
               virtual_chunk_id_z[k] -= num_chunks[2]) { /* NA */}

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 y_dir,
                                                 k);

               this_chunk->move( 0, 0, -num_chunks[2]);
            }
         }

      }

      if (virtual_chunk_id_z[k] < 0)
      {
         for (;virtual_chunk_id_z[k] < 0; virtual_chunk_id_z[k] += num_chunks[2]) { /* NA */}

         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
            {

               // shift the local chunk position
               Chunk *this_chunk = access_chunk( x_dir,
                                                 y_dir,
                                                 k);

               this_chunk->move( 0, 0, num_chunks[2]);
            }
         }
      }

      /*
      ** Update the virtual-to-physical grid mapping
      */
      physical_chunk_id_z[virtual_chunk_id_z[k]] = k;
   }
}

/*
** function name: get_abs_element from: Map
**
** returns the block element at the absolute position.
** assumes a toroidal topology with the chunks so it will
** return a block element modulo the number of chunks
*/
int Map::get_abs_element( int* position_in)
{
   int physical_chunk_position[3];
   get_physical_chunk_position( position_in, physical_chunk_position );

   Chunk *this_chunk = access_chunk( physical_chunk_position[0],
                                     physical_chunk_position[1],
                                     physical_chunk_position[2]);

   int element_position[3];
   get_relative_element_position( position_in, element_position );

   int element = this_chunk->get_block( element_position );

   return element;
}

/*
** function name: get_physical_chunk_position from: Map
*/
void Map::get_physical_chunk_position( int* abs_position,
                                       int* physical_chunk_position )
{
   if (abs_position[0] >= 0)
   {
      physical_chunk_position[0] = ((abs_position[0] / num_chunk_elements[0]) +
                                     num_chunks[0] / 2) % num_chunks[0];
   }
   else
   {
      physical_chunk_position[0] = (((abs_position[0] + 1) / num_chunk_elements[0]) +
                                     num_chunks[0] / 2 - 1) % num_chunks[0];
   }
   if (physical_chunk_position[0] < 0) physical_chunk_position[0] += num_chunks[0];

   if (abs_position[1] >= 0)
   {
      physical_chunk_position[1] = ((abs_position[1] / num_chunk_elements[1]) +
                                     num_chunks[1] / 2) % num_chunks[1];
   }
   else
   {
      physical_chunk_position[1] = (((abs_position[1] + 1) / num_chunk_elements[1]) +
                                     num_chunks[1] / 2 - 1) % num_chunks[1];
   }
   if (physical_chunk_position[1] < 0) physical_chunk_position[1] += num_chunks[1];

   if (abs_position[2] >= 0)
   {
      physical_chunk_position[2] = ((abs_position[2] / num_chunk_elements[2]) +
                                     num_chunks[2] / 2) % num_chunks[2];
   }
   else
   {
      physical_chunk_position[2] = (((abs_position[2] + 1) / num_chunk_elements[2]) +
                                     num_chunks[2] / 2 - 1) % num_chunks[2];
   }
   if (physical_chunk_position[2] < 0) physical_chunk_position[2] += num_chunks[2];

}

void Map::get_relative_element_position( int* position_in, int* element_position )
{
   element_position[0] = position_in[0] % num_chunk_elements[0];
   element_position[1] = position_in[1] % num_chunk_elements[1];
   element_position[2] = position_in[2] % num_chunk_elements[2];

   if (element_position[0] < 0) element_position[0] += num_chunk_elements[0];
   if (element_position[1] < 0) element_position[1] += num_chunk_elements[1];
   if (element_position[2] < 0) element_position[2] += num_chunk_elements[2];

}

void Map::diagnostics( int *position_in, Text *text)
{
   int physical_chunk_position[3];
   get_physical_chunk_position( position_in, physical_chunk_position );

   int element_position[3];
   get_relative_element_position( position_in, element_position );

   text->populate("position in: ");
   text->populate( position_in[0] );
   text->populate( ", ");
   text->populate( position_in[1] );
   text->populate( ", ");
   text->populate( position_in[2] );
   text->populate( ", ");
   text->new_line();
   text->populate("Physical Chunk ids x: ");
   for (int k = 0; k < num_chunks[0]; k++)
   {
      text->populate( physical_chunk_id_x[k] );
      text->populate( ", ");
   }
   text->new_line();
   text->populate("Physical Chunk: ");
   text->populate( physical_chunk_position[0] );
   text->populate( ", ");
   text->populate( physical_chunk_position[1] );
   text->populate( ", ");
   text->populate( physical_chunk_position[2] );
   text->new_line();
   text->populate("Relative element: ");
   text->populate( element_position[0] );
   text->populate( ", ");
   text->populate( element_position[1] );
   text->populate( ", ");
   text->populate( element_position[2] );

   
   Chunk *this_chunk = access_chunk( physical_chunk_position[0],
                                     physical_chunk_position[1],
                                     physical_chunk_position[2]);

   int element = this_chunk->get_block( element_position );

   text->new_line();
   text->populate("Chunk element value: ");
   text->populate( element );

   int abs_pos_id[3];
   this_chunk->get_abs_pos_id( abs_pos_id );
   text->new_line();
   text->populate("Chunk absolute position: ");
   text->populate( abs_pos_id[0] );
   text->populate( ", ");
   text->populate( abs_pos_id[1] );
   text->populate( ", ");
   text->populate( abs_pos_id[2] );

}

/*
** function name: debug_info from: Map
*/
void Map::debug_info( void )
{

   std::cout << "virtual id 0 -> physical grid id (" <<
                 physical_chunk_id_x[0] << ", "       <<
                 physical_chunk_id_y[0] << ", "       <<
                 physical_chunk_id_z[0] << ")"        <<
                 std::endl;

   std::cout << "physical -> virtual grid id:" << std::endl;
   std::cout << "x: ";
   for (int k = 0; k < num_chunks[0]; k++)
   {
      std::cout << "(" << k << "," << virtual_chunk_id_x[k] << "), ";
   }
   std::cout << std::endl;

   std::cout << "y: ";
   for (int k = 0; k < num_chunks[1]; k++)
   {
      std::cout << "(" << k << "," << virtual_chunk_id_y[k] << "), ";
   }
   std::cout << std::endl;

   std::cout << "z: ";
   for (int k = 0; k < num_chunks[2]; k++)
   {
      std::cout << "(" << k << "," << virtual_chunk_id_z[k] << "), ";
   }
   std::cout << std::endl;

   Chunk *this_chunk = access_chunk( physical_chunk_id_x[num_chunks[0]/2],
                                     physical_chunk_id_y[num_chunks[1]/2],
                                     physical_chunk_id_z[num_chunks[2]/2]);

   int abs_pos_id[3];
   this_chunk->get_abs_pos_id( abs_pos_id );
   std::cout << "absolute chunk position id at center = (" <<
                 abs_pos_id[0] << ", "            <<
                 abs_pos_id[1] << ", "            <<
                 abs_pos_id[2] << ")"             <<
                 std::endl;

}
