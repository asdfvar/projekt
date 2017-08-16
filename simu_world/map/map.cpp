#include "map.h"
#include <iostream>
#include <cstdlib>
#include "draw_block.h"
#include "fileio.h"
#include <cmath>
#include "read_chunk.h"
#include "diagnostics.h"

/*
** constructor name: Map
*/
Map::Map( pthread_barrier_t* IO_barrier_in,
          pthread_barrier_t* update_barrier_in ) :
          workspace(4294967296)
{

   IO_barrier     = IO_barrier_in;
   update_barrier = update_barrier_in;

   num_chunks_x = 9;
   num_chunks_y = 9;
   num_chunks_z = 9;

   /*
   ** build the chunks
   */
   chunk_dim_x = 13;
   chunk_dim_y = 13;
   chunk_dim_z = 13;

   int total_num_chunk_elements = chunk_dim_x *
                                  chunk_dim_y *
                                  chunk_dim_z;

   write_permissions = new bool[ total_num_chunk_elements ];

   /*
   ** initialize the mapping from the virtual grid index
   ** to the physical grid index
   */
   physical_chunk_id_x = new int[num_chunks_x];
   physical_chunk_id_y = new int[num_chunks_y];
   physical_chunk_id_z = new int[num_chunks_z];

   for (int k = 0; k < num_chunks_x; k++) physical_chunk_id_x[k] = k;
   for (int k = 0; k < num_chunks_y; k++) physical_chunk_id_y[k] = k;
   for (int k = 0; k < num_chunks_z; k++) physical_chunk_id_z[k] = k;

   /*
   ** initialize the mapping from the physical grid
   ** to the virtual grid index
   */
   virtual_chunk_id_x = new int[num_chunks_x];
   virtual_chunk_id_y = new int[num_chunks_y];
   virtual_chunk_id_z = new int[num_chunks_z];

   for (int k = 0; k < num_chunks_x; k++) virtual_chunk_id_x[k] = k;
   for (int k = 0; k < num_chunks_y; k++) virtual_chunk_id_y[k] = k;
   for (int k = 0; k < num_chunks_z; k++) virtual_chunk_id_z[k] = k;

   /*
   ** create the save directory
   */
   fio::directory();

   dim_x = chunk_dim_x * num_chunks_x;
   dim_y = chunk_dim_y * num_chunks_y;
   dim_z = chunk_dim_z * num_chunks_z;

   map_pos_x = 0.0f;
   map_pos_y = 0.0f;
   map_pos_z = 0.0f;

   int total_dim = dim_x * dim_y * dim_z;

   int max_chunk_elements = (chunk_dim_x > chunk_dim_y) ?
                             chunk_dim_x : chunk_dim_y;
   if (max_chunk_elements < chunk_dim_z) max_chunk_elements = chunk_dim_z;

   int max_dim = (dim_x > dim_y) ? dim_x : dim_y;
   if (max_dim < dim_z) max_dim = dim_z;

   blocks  = new int[ total_dim ];
   io_ids  = new int[ total_num_chunk_elements ];

   Workspace l_workspace = workspace;
   int* buf = l_workspace.reserve<int>( max_dim * max_dim * max_chunk_elements );

   for (int k = 0, ind = 0; k < num_chunks_z; k++)
   {
      for (int j = 0; j < num_chunks_y; j++)
      {
         for (int i = 0; i < num_chunks_x; i++, ind++)
         {
            std::string filename = create_filename( i, j, k );

            bool file_exists =
               read_chunk( filename, buf, total_num_chunk_elements );

            if( !file_exists )
            {
               generate_chunk( buf,
                               map_pos_x + chunk_dim_x * i,
                               map_pos_y + chunk_dim_y * j,
                               map_pos_z + chunk_dim_z * k,
                               chunk_dim_x,
                               chunk_dim_y,
                               chunk_dim_z );

               std::cout << filename << " does not exist."
                  << " chunk created." << std::endl;
               write_permissions[ind] = true;
            }
            else
            {
               std::cout << "reading " << filename << std::endl;
               write_permissions[ind] = false;
            }

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
   workspace.finalize();

   delete[] blocks;
   delete[] io_ids;
   delete[] write_permissions;
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
      physical_chunk_position[0] = ((abs_position[0] / chunk_dim_x) +
                                     num_chunks_x / 2) % num_chunks_x;
   }
   else
   {
      physical_chunk_position[0] = (((abs_position[0] + 1) / chunk_dim_x) +
                                     num_chunks_x / 2 - 1) % num_chunks_x;
   }
   if (physical_chunk_position[0] < 0) physical_chunk_position[0] += num_chunks_x;

   if (abs_position[1] >= 0)
   {
      physical_chunk_position[1] = ((abs_position[1] / chunk_dim_y) +
                                     num_chunks_y / 2) % num_chunks_y;
   }
   else
   {
      physical_chunk_position[1] = (((abs_position[1] + 1) / chunk_dim_y) +
                                     num_chunks_y / 2 - 1) % num_chunks_y;
   }
   if (physical_chunk_position[1] < 0) physical_chunk_position[1] += num_chunks_y;

   if (abs_position[2] >= 0)
   {
      physical_chunk_position[2] = ((abs_position[2] / chunk_dim_z) +
                                     num_chunks_z / 2) % num_chunks_z;
   }
   else
   {
      physical_chunk_position[2] = (((abs_position[2] + 1) / chunk_dim_z) +
                                     num_chunks_z / 2 - 1) % num_chunks_z;
   }
   if (physical_chunk_position[2] < 0) physical_chunk_position[2] += num_chunks_z;

}

/*
** function name: get_relative_element_position from: Map
*/
void Map::get_relative_element_position( int* position_in, int* element_position )
{
   element_position[0] = position_in[0] % chunk_dim_x;
   element_position[1] = position_in[1] % chunk_dim_y;
   element_position[2] = position_in[2] % chunk_dim_z;

   if ( element_position[0] < 0 ) element_position[0] += chunk_dim_x;
   if ( element_position[1] < 0 ) element_position[1] += chunk_dim_y;
   if ( element_position[2] < 0 ) element_position[2] += chunk_dim_z;
}

/*
** function name: diagnostics from: Map
*/
void Map::diagnostics( int *position_in )
{
   int physical_chunk_position[3];
   get_physical_chunk_position( position_in, physical_chunk_position );

   int element_position[3];
   get_relative_element_position( position_in, element_position );

#ifdef DEBUG
   if (screen_feedback)
   {
      g_text.populate( "position in: ");
      g_text.populate( position_in[0] );
      g_text.populate( ", ");
      g_text.populate( position_in[1] );
      g_text.populate( ", ");
      g_text.populate( position_in[2] );
      g_text.populate( ", ");
      g_text.new_line();
      g_text.populate("Physical Chunk ids x: ");
      for (int k = 0; k < num_chunks_x; k++)
      {
         g_text.populate( physical_chunk_id_x[k] );
         g_text.populate( ", ");
      }
      g_text.new_line();
      g_text.populate("Physical Chunk: ");
      g_text.populate( physical_chunk_position[0] );
      g_text.populate( ", ");
      g_text.populate( physical_chunk_position[1] );
      g_text.populate( ", ");
      g_text.populate( physical_chunk_position[2] );
      g_text.new_line();
      g_text.populate("Relative element: ");
      g_text.populate( element_position[0] );
      g_text.populate( ", ");
      g_text.populate( element_position[1] );
      g_text.populate( ", ");
      g_text.populate( element_position[2] );
      g_text.new_line();
      g_text.populate("map position: ");
      g_text.populate( map_pos_x );
      g_text.populate(",");
      g_text.populate( map_pos_y );
      g_text.populate(",");
      g_text.populate( map_pos_z );

      int block_position[3];

      int num_chunks_x_2 = num_chunks_x / 2;
      block_position[0] = (position_in[0] - map_pos_x) +
         (chunk_dim_x *
          num_chunks_x_2);

      int num_chunks_y_2 = num_chunks_y / 2;
      block_position[1] = (position_in[1] - map_pos_y) +
         (chunk_dim_y *
          num_chunks_y_2);

      int num_chunks_z_2 = num_chunks_z / 2;
      block_position[2] = (position_in[2] - map_pos_z) +
         (chunk_dim_z *
          num_chunks_z_2);

      int ind = block_position[0]                 +
         block_position[1] * dim_x         +
         block_position[2] * dim_x * dim_y;

      g_text.new_line();
      g_text.populate("block index: ");
      g_text.populate( ind );
      g_text.populate(" = ");
      g_text.populate( block_position[0] );
      g_text.populate(", ");
      g_text.populate( block_position[1] );
      g_text.populate(", ");
      g_text.populate( block_position[2] );
      g_text.new_line();
      g_text.populate("Chunk element value: ");
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
      g_text.populate( blocks[ind++] );
   }
#endif
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
** function name: get_chunk from: Map
*/
void Map::get_chunk( int* dst,
                     int  chunk_x,
                     int  chunk_y,
                     int  chunk_z,
                     int  aug )
{
   int ind = 0;
   for (int k = chunk_z * chunk_dim_z - aug;
        k < (chunk_z + 1) * chunk_dim_z + aug;
        k++)
   {
      for (int j = chunk_y * chunk_dim_y - aug;
           j < (chunk_y + 1) * chunk_dim_y + aug;
           j++)
      {
         for (int i = chunk_x * chunk_dim_x - aug;
              i < (chunk_x + 1) * chunk_dim_x + aug;
              i++, ind++)
         {
            if (i < 0 || i >= dim_x || j < 0 || j >= dim_y || k < 0 || k >= dim_z)
            {
               dst[ind] = 0;
            }
            else
            {
               dst[ind] = blocks[ i +
                                  j * dim_x +
                                  k * dim_x * dim_y ];
            }
         }
      }
   }
}
