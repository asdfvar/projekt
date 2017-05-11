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
Map::Map( pthread_barrier_t* IO_barrier_in )
{

   IO_barrier = IO_barrier_in;

   num_chunks[0] = 13;
   num_chunks[1] = 13;
   num_chunks[2] = 13;

   chunks = new Chunks( num_chunks );

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

   /*
   ** build the chunks
   */
   int num_chunk_elements[3] = { 5, 5, 5 };
   int total_num_chunk_elements = num_chunk_elements[0] *
                                  num_chunk_elements[1] *
                                  num_chunk_elements[2];
   int abs_pos_id[3];

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

   blocks = new int[ total_dim ];

   create_random( blocks, total_dim );

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
           chunks->insert_chunk( new Chunk(ind, abs_pos_id, num_chunk_elements, position) );

         }
      }
   }

   chunks->set_base();

   int max_chunk_elements = (num_chunk_elements[0] > num_chunk_elements[1]) ?
                             num_chunk_elements[0] : num_chunk_elements[1];
   if (max_chunk_elements < num_chunk_elements[2]) max_chunk_elements = num_chunk_elements[2];
   int max_dim = (dim_x > dim_y) ? dim_x : dim_y;
   if (max_dim < dim_z) max_dim = dim_z;

   buf = new float[ max_dim * max_dim * max_chunk_elements ];

   queue = new Queue( total_num_chunk_elements );

   // wait for the IO thread to have finished its initialization before continuing
   //pthread_barrier_wait( IO_barrier );

   int junk[125];
   for (int ind = 0; ind < 125; ind++) junk[ind] = 1;
   set_chunk( junk,
              0,
              0,
              0 );

   std::cout  << "finished creating map" << std::endl;

}

/*
** destructor name: ~Map
*/
Map::~Map(void)
{
   delete[] blocks;
   delete[] buf;
   delete   chunks;
   delete   queue;

   delete[] physical_chunk_id_x;
   delete[] physical_chunk_id_y;
   delete[] physical_chunk_id_z;

   delete[] virtual_chunk_id_x;
   delete[] virtual_chunk_id_y;
   delete[] virtual_chunk_id_z;

}

/*
** function name: set_phys_chunk_color from: Map
*/
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
** function name: get_physical_chunk_position from: Map
*/
void Map::get_physical_chunk_position( int* abs_position,
                                       int* physical_chunk_position )
{
   int num_chunk_elements[3];
   Chunk *chunk = access_chunk( 0, 0, 0 );
   num_chunk_elements[0] = chunk->get_dimension( 0 );
   num_chunk_elements[1] = chunk->get_dimension( 1 );
   num_chunk_elements[2] = chunk->get_dimension( 2 );

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
   int num_chunk_elements[3];
   Chunk *chunk = access_chunk( 0, 0, 0 );
   num_chunk_elements[0] = chunk->get_dimension( 0 );
   num_chunk_elements[1] = chunk->get_dimension( 1 );
   num_chunk_elements[2] = chunk->get_dimension( 2 );

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
                 abs_pos_id[0] << ", "                     <<
                 abs_pos_id[1] << ", "                     <<
                 abs_pos_id[2] << ")"                      <<
                 std::endl;

}

/*
** function name: set_chunk from: Map
*/
void Map::set_chunk( int* src,
                     int chunk_x,
                     int chunk_y,
                     int chunk_z )
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
