#include "map.h"
#include <iostream>
#include <cstdlib>
#include "draw_block.h"
#include "fileio.h"
#include <cmath>
#include "read_chunk.h"

/*
** constructor name: Map
*/
Map::Map( pthread_barrier_t* IO_barrier_in,
          pthread_barrier_t* update_barrier_in )
{

   IO_barrier     = IO_barrier_in;
   update_barrier = update_barrier_in;

   num_chunks[0] = 3;
   num_chunks[1] = 3;
   num_chunks[2] = 3;

   /*
   ** build the chunks
   */
   num_chunk_elements[0] = 3;
   num_chunk_elements[1] = 3;
   num_chunk_elements[2] = 3;

   int total_num_chunk_elements = num_chunk_elements[0] *
                                  num_chunk_elements[1] *
                                  num_chunk_elements[2];

   /*
   ** initialize the mapping from the virtual grid index
   ** to the physical grid index
   */
   physical_chunk_id_x = new int[num_chunks[0]];
   physical_chunk_id_y = new int[num_chunks[1]];
   physical_chunk_id_z = new int[num_chunks[2]];

   for (int k = 0; k < num_chunks[0]; k++) physical_chunk_id_x[k] = k;
   for (int k = 0; k < num_chunks[1]; k++) physical_chunk_id_y[k] = k;
   for (int k = 0; k < num_chunks[2]; k++) physical_chunk_id_z[k] = k;

   /*
   ** initialize the mapping from the physical grid
   ** to the virtual grid index
   */
   virtual_chunk_id_x = new int[num_chunks[0]];
   virtual_chunk_id_y = new int[num_chunks[1]];
   virtual_chunk_id_z = new int[num_chunks[2]];

   for (int k = 0; k < num_chunks[0]; k++) virtual_chunk_id_x[k] = k;
   for (int k = 0; k < num_chunks[1]; k++) virtual_chunk_id_y[k] = k;
   for (int k = 0; k < num_chunks[2]; k++) virtual_chunk_id_z[k] = k;

   /*
   ** create the save directory
   */
   fio::directory();

   dim_x = num_chunk_elements[0] * num_chunks[0];
   dim_y = num_chunk_elements[1] * num_chunks[1];
   dim_z = num_chunk_elements[2] * num_chunks[2];

   chunk_dim_x = num_chunk_elements[0];
   chunk_dim_y = num_chunk_elements[1];
   chunk_dim_z = num_chunk_elements[2];

   map_pos_x = 0.0f;
   map_pos_y = 0.0f;
   map_pos_z = 0.0f;

   int total_dim = dim_x * dim_y * dim_z;

   int max_chunk_elements = (num_chunk_elements[0] > num_chunk_elements[1]) ?
                             num_chunk_elements[0] : num_chunk_elements[1];
   if (max_chunk_elements < num_chunk_elements[2]) max_chunk_elements = num_chunk_elements[2];

   int max_dim = (dim_x > dim_y) ? dim_x : dim_y;
   if (max_dim < dim_z) max_dim = dim_z;

   blocks  = new int[ total_dim ];
   io_ids  = new int[ total_num_chunk_elements ];
   buf     = new int[ max_dim * max_dim * max_chunk_elements ];

   for (int k = 0, ind_z = -num_chunks[2]/2, ind = 0; ind_z <= num_chunks[2]/2; ind_z++, k++)
   {
      for (int j = 0, ind_y = -num_chunks[1]/2; ind_y <= num_chunks[1]/2; ind_y++, j++)
      {
         for (int i = 0, ind_x = -num_chunks[0]/2; ind_x <= num_chunks[0]/2; ind_x++, ind++, i++)
         {
           create_random ( buf, total_num_chunk_elements );
           set_chunk ( buf, i, j, k );
         }
      }
   }

   queue = new Queue( total_num_chunk_elements );

   std::cout  << "finished creating map" << std::endl;

}

/*
** destructor name: ~Map
*/
Map::~Map(void)
{
   delete[] blocks;
   delete[] io_ids;
   delete[] buf;
   delete   queue;

   delete[] physical_chunk_id_x;
   delete[] physical_chunk_id_y;
   delete[] physical_chunk_id_z;

   delete[] virtual_chunk_id_x;
   delete[] virtual_chunk_id_y;
   delete[] virtual_chunk_id_z;

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

/*
** function name: get_relative_element_position from: Map
*/
void Map::get_relative_element_position( int* position_in, int* element_position )
{
   element_position[0] = position_in[0] % num_chunk_elements[0];
   element_position[1] = position_in[1] % num_chunk_elements[1];
   element_position[2] = position_in[2] % num_chunk_elements[2];

   if ( element_position[0] < 0 ) element_position[0] += num_chunk_elements[0];
   if ( element_position[1] < 0 ) element_position[1] += num_chunk_elements[1];
   if ( element_position[2] < 0 ) element_position[2] += num_chunk_elements[2];
}

/*
** function name: diagnostics from: Map
*/
void Map::diagnostics( int *position_in, Text *text)
{
   int physical_chunk_position[3];
   get_physical_chunk_position( position_in, physical_chunk_position );

   int element_position[3];
   get_relative_element_position( position_in, element_position );

   text->populate( "position in: ");
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
   text->new_line();
   text->populate("map position: ");
   text->populate( map_pos_x );
   text->populate(",");
   text->populate( map_pos_y );
   text->populate(",");
   text->populate( map_pos_z );
   
   int block_position[3];
   block_position[0] = (position_in[0] % num_chunk_elements[0] + num_chunk_elements[0]) %
                        num_chunk_elements[0] + num_chunk_elements[0] * (num_chunks[0] / 2);
   block_position[1] = (position_in[1] % num_chunk_elements[1] + num_chunk_elements[1]) %
                        num_chunk_elements[1] + num_chunk_elements[1] * (num_chunks[1] / 2);
   block_position[2] = (position_in[2] % num_chunk_elements[2] + num_chunk_elements[2]) %
                        num_chunk_elements[2] + num_chunk_elements[2] * (num_chunks[2] / 2);

   block_position[0] = (position_in[0] - map_pos_x) +
                       num_chunk_elements[0] *
                       num_chunks[0] / 2;

   block_position[1] = (position_in[1] - map_pos_y) +
                       num_chunk_elements[1] *
                       num_chunks[1] / 2;

   block_position[2] = (position_in[2] - map_pos_z) +
                       num_chunk_elements[2] *
                       num_chunks[2] / 2;

   int ind = block_position[0]                 +
             block_position[1] * dim_x         +
             block_position[2] * dim_x * dim_y;

   text->new_line();
   text->populate("block index: ");
   text->populate( ind );
   text->populate(" = ");
   text->populate( block_position[0] );
   text->populate(", ");
   text->populate( block_position[1] );
   text->populate(", ");
   text->populate( block_position[2] );
   text->new_line();
   text->populate("Chunk element value: ");
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
   text->populate( blocks[ind++] );
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

}

/*
** function name: set_chunk from: Map
*/
void Map::set_chunk( int* src,
                     int  chunk_x,
                     int  chunk_y,
                     int  chunk_z )
{
   int ind = 0;
   for (int k = chunk_z * chunk_dim_z; k < (chunk_z + 1) * chunk_dim_z; k++)
   {
      for (int j = chunk_y * chunk_dim_y; j < (chunk_y + 1) * chunk_dim_y; j++)
      {
         for (int i = chunk_x * chunk_dim_x; i < (chunk_x + 1) * chunk_dim_x; i++, ind++)
         {
            blocks[ i +
                    j * dim_x +
                    k * dim_x * dim_y ] = src[ind];
         }
      }
   }
}

/*
** function name: set_chunk from: Map
*/
void Map::get_chunk( int* dst,
                     int  chunk_x,
                     int  chunk_y,
                     int  chunk_z )
{
   int ind = 0;
   for (int k = chunk_z * chunk_dim_z; k < (chunk_z + 1) * chunk_dim_z; k++)
   {
      for (int j = chunk_y * chunk_dim_y; j < (chunk_y + 1) * chunk_dim_y; j++)
      {
         for (int i = chunk_x * chunk_dim_x; i < (chunk_x + 1) * chunk_dim_x; i++, ind++)
         {
            dst[ind] = blocks[ i +
                               j * dim_x +
                               k * dim_x * dim_y ];
         }
      }
   }
}
