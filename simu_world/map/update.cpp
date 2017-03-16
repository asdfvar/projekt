#include "map.h"
#include "chunk.h"
#include "fileio.h"
#include <sstream>

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

            //TODO: pass the queue into the chunk update. from there,
            //      it will add the new chunk to the queue to be saved
            this_chunk->update();

            bool has_moved = this_chunk->has_moved;
            bool changed   = this_chunk->changed;

#if 0
            //TODO: adjust this logic appropriately once the previous stuff has been
            //      flushed out
            if ( has_moved && changed )
            {
               this_chunk->has_moved = false;
#if 1
               Chunk* chunk = new Chunk();
              *chunk = *this_chunk;
#else
int abs_pos_id_in[3] = {1, 1, 1};
int chunk_dim_in[3] = {5, 5, 5};
float position_in[3] = {0.0f, 0.0f, 0.0f};
Chunk* chunk = new Chunk( 0,
       abs_pos_id_in,
       chunk_dim_in,
       position_in);
#endif
std::cout << "new chunk at " << chunk << " original chunk at " << this_chunk << std::endl;
               queue.new_chunk( chunk );
delete chunk;
            }
#endif
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

/*
** function name: write_chunk from Chunk
*/
void Chunk::write_chunk( void )
{

      std::string chunk_name = "saves/chunk_";
      std::ostringstream id_str;

      if (prev_abs_pos_id[0] < 0)
      {
         id_str << -prev_abs_pos_id[0];
         chunk_name += "n";
      }
      else
      {
         id_str << prev_abs_pos_id[0];
      }
      chunk_name += id_str.str();
      chunk_name += "_";

      id_str.clear();
      id_str.str("");
      if (prev_abs_pos_id[1] < 0)
      {
         id_str << -prev_abs_pos_id[1];
         chunk_name += "n";
      }
      else
      {
         id_str << prev_abs_pos_id[1];
      }
      chunk_name += id_str.str();
      chunk_name += "_";

      id_str.clear();
      id_str.str("");
      if (prev_abs_pos_id[2] < 0)
      {
         id_str << -prev_abs_pos_id[2];
         chunk_name += "n";
      }
      else
      {
         id_str << prev_abs_pos_id[2];
      }
      chunk_name += id_str.str();

      std::cout << "writing chunk (" << prev_abs_pos_id[0] << ", "
                                     << prev_abs_pos_id[1] << ", "
                                     << prev_abs_pos_id[2] << ")"
                                     << std::endl;

      fio::write( chunk_name,
                  0,
                 (char*)blocks,
                  chunk_dim[0] *
                  chunk_dim[1] *
                  chunk_dim[2] * sizeof(chunk_dim[0]));
}

/*
** function name: generate_chunk from Chunk
*/
void Chunk::generate_chunk( void )
{
#ifdef __linux__
   std::string chunk_name = "saves/chunk_";
#elif _WIN32
   std::string chunk_name = "saves\chunk_";
#else
   std::cout << __FILE__ << ":" << __LINE__ << ": unknown OS" << std::endl;
#endif

   std::ostringstream id_str;
   if (abs_pos_id[0] < 0)
   {
      id_str << -abs_pos_id[0];
      chunk_name += "n";
   }
   else
   {
      id_str << abs_pos_id[0];
   }
   chunk_name += id_str.str();
   chunk_name += "_";

   id_str.clear();
   id_str.str("");
   if (abs_pos_id[1] < 0)
   {
      id_str << -abs_pos_id[1];
      chunk_name += "n";
   }
   else
   {
      id_str << abs_pos_id[1];
   }
   chunk_name += id_str.str();
   chunk_name += "_";

   id_str.clear();
   id_str.str("");
   if (abs_pos_id[2] < 0)
   {
      id_str << -abs_pos_id[2];
      chunk_name += "n";
   }
   else
   {
      id_str << abs_pos_id[2];
   }
   chunk_name += id_str.str();

   if( !fio::read( chunk_name,       // path and file name included
                   0,                // offset in bytes from the beginning of file
                   (void*)blocks,    // buffer to hold the data
                   chunk_dim[0] *
                      chunk_dim[1] *
                      chunk_dim[2] *
                      sizeof(chunk_dim[0])) ) // number of bytes to read
   {
      std::cout << "new chunk (" << abs_pos_id[0] << ", "
                                 << abs_pos_id[1] << ", "
                                 << abs_pos_id[2] << ")"
                                 << std::endl;
      create_random();
//      create_flat();
//      create_solid();
      changed         = true;
   }
   else
   {
      std::cout << "reading chunk (" << abs_pos_id[0] << ", "
                                     << abs_pos_id[1] << ", "
                                     << abs_pos_id[2] << ")"
                                     << std::endl;
      changed         = false;
      first_populated = false;
   }
}
