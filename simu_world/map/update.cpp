#include "map.h"
#include "fileio.h"
#include "queue.h"
#include "read_chunk.h"
#include <sstream>

/*
** function name: update from: Map
** Update chunks from the IO thread
**
*/
void Map::update( void )
{

   /*
    ** note to self: this may be disabled during
    ** development to mitigate excessive writes to the SSD
    */

   // write all queued chunks to disk
   queue->write_all();

   // for each queued chunk to read, read in chunk data
   // if chunk file doesn't exist, create the chunk
   // this logic currently exists in shift.cpp.
   for ( QNode_vector<int>* chunk = readQueue.pop();
         chunk != NULL;
         chunk = readQueue.pop() )
   {

      if (chunk != NULL)
      {
         int abs_chunk_x = chunk->read( 0 );
         int abs_chunk_y = chunk->read( 1 );
         int abs_chunk_z = chunk->read( 2 );

         std::cout << "absolute chunk = "
            << abs_chunk_x << ", "
            << abs_chunk_y << ", "
            << abs_chunk_z;

         int rel_chunk_x = abs_chunk_x - map_pos_x / chunk_dim_x;
         int rel_chunk_y = abs_chunk_y - map_pos_y / chunk_dim_y;
         int rel_chunk_z = abs_chunk_z - map_pos_z / chunk_dim_z;

         std::cout << ". relative chunk = "
            << rel_chunk_x << ", "
            << rel_chunk_y << ", "
            << rel_chunk_z
            << std::endl;

         std::string filename = create_filename( abs_chunk_x,
               abs_chunk_y,
               abs_chunk_z );


         int ind = rel_chunk_x +
            rel_chunk_y * num_chunks_x +
            rel_chunk_z * num_chunks_x * num_chunks_y;

         int total_num_chunk_elements = chunk_dim_x * chunk_dim_y * chunk_dim_z;
         // TODO: handle this properly
         int buffer[20*20*20];

         bool file_exists =
            read_chunk( filename,
                  buffer,
                  total_num_chunk_elements );

         if( !file_exists )
         {
            generate_chunk( buffer,
                  chunk_dim_x * abs_chunk_x,
                  chunk_dim_y * abs_chunk_y,
                  chunk_dim_z * abs_chunk_z,
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

         // copy the contents of buffer into the block
         set_chunk( buffer,
               rel_chunk_x,
               rel_chunk_y,
               rel_chunk_z );
      }

      delete chunk;
   }

}
