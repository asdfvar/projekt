#include "map.h"
#include "chunk.h"

/*
** function name: update from: Map
** Update chunks from the IO thread
**
** TODO: create a queueing system where
**       chunks that are to be written to file
**       are copied to a queue and the chunks
**       in the queue are written to file. chunks
**       that attempt to read from file that is
**       not yet written to must wait
*/
void Map::update( void )
{

   for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
   {
      for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
      {
         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
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
** function name: update from: Chunk
**
** write old data to file and read new
** data to memory if it exists. Otherwise, create it.
** chunks are written to file if they leave the field
** of view (a.k.a. chunks are reassigned).
*/
void Chunk::update( void )
{
   if ( ( changed && !valid ) || first_populated )
   {
      /*
      ** write old data to file
      */
      write_chunk();

      changed         = false;
      first_populated = false;
   }

   if ( !valid )
   {
      /*
      ** Attempt to read from file
      */
      generate_chunk();

      valid = true;
   }
}
