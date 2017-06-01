#include "map.h"
#include "chunk.h"
#include "fileio.h"
#include "queue.h"
#include <sstream>

/*
** function name: update from: Map
** Update chunks from the IO thread
**
*/
void Map::update( void )
{

#ifdef BLOCKS

// generate chunk
#ifdef __linux__
   std::string chunk_name = "saves/chunk_";
#elif _WIN32
   std::string chunk_name = "saves\chunk_";
#else
   std::cout << __FILE__ << ":" << __LINE__ << ": unknown OS" << std::endl;
#endif

   //FIXME
   int abs_pos_id[3] = { 0, 0, 0 };

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

#if 0
   int total_num_chunk_el = num_chunk_elements[0] *
                            num_chunk_elements[1] *
                            num_chunk_elements[2];

   bool file_exists =
         fio::read( chunk_name,       // path and file name included
                    0,                // offset in bytes from the beginning of file
                    (void*)buf,       // buffer to hold the data
                    total_num_chunk_el * sizeof(int) );

   if( !file_exists ) // number of bytes to read
   {
      changed = true;
      int chunk_dim_tot = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
      for (int ind = 0; ind < chunk_dim_tot; ind++)
      {
         int random_num = std::rand() % 100;
         if (random_num == 0)
            blocks[ind] = 1;
         else
            blocks[ind] = 0;
      }
   }
   else
   {
      changed         = false;
      first_populated = false;
   }
#endif

#else

   /*
   ** write all chunks in queue to file
   */
   queue->write_chunk();

   for (int z_dir = 0; z_dir < num_chunks[2]; z_dir++)
   {
      for (int y_dir = 0; y_dir < num_chunks[1]; y_dir++)
      {
         for (int x_dir = 0; x_dir < num_chunks[0]; x_dir++)
         {

            Chunk *this_chunk = access_chunk( x_dir,
                                              y_dir,
                                              z_dir );

            this_chunk->update( queue );

         }
      }
   }

   /* TODO: create logic to have the queue manage reading data from
   **       disc as well and have the chunks pop the data off the queue
   **       when they are ready to read them. eventually, make the above
   **       read-operation OBE
   */
#endif

}

/*
** function name: update from: Chunk
**
** write old data to file and read new
** data to memory if it exists. Otherwise, create it.
** chunks are written to file if they leave the field
** of view (a.k.a. chunks are reassigned).
*/
#ifndef BLOCKS
void Chunk::update( Queue* queue )
{

   if ( !valid )
   {
      /*
      ** Attempt to read from file
      */
      generate_chunk();

      valid = true;
   }

   if ( first_populated )
   {
      int total_chunk_dim = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
      int* chunk_elements = new int[total_chunk_dim];

      for( int k = 0; k < total_chunk_dim; k++) chunk_elements[k] = blocks[k];

      queue->new_chunk( chunk_elements,
                        prev_abs_pos_id_new[0],
                        prev_abs_pos_id_new[1],
                        prev_abs_pos_id_new[2] );

      first_populated = false;
   }

}
#endif

#ifndef BLOCKS
/*
** function name: populate from: Chunk
**
*/
void Chunk::populate( Queue* queue )
{

   if ( reassigned )
   {
      int  total_chunk_dim = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
      int* chunk_elements  = new int[total_chunk_dim];

      for( int k = 0; k < total_chunk_dim; k++) chunk_elements[k] = blocks[k];

      if ( changed )
      {
         queue->new_chunk( chunk_elements,
                           prev_abs_pos_id_new[0],
                           prev_abs_pos_id_new[1],
                           prev_abs_pos_id_new[2] );
      }

      reassigned = false;
   }

   prev_abs_pos_id_new[0] = abs_pos_id[0];
   prev_abs_pos_id_new[1] = abs_pos_id[1];
   prev_abs_pos_id_new[2] = abs_pos_id[2];

}
#endif

// TODO: remove this
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

   bool file_exists =
         fio::read( chunk_name,       // path and file name included
                   0,                // offset in bytes from the beginning of file
                   (void*)blocks,    // buffer to hold the data
                   chunk_dim[0] *
                      chunk_dim[1] *
                      chunk_dim[2] *
                      sizeof(chunk_dim[0]));

   if( !file_exists ) // number of bytes to read
   {
      create_random();
//      create_flat();
//      create_solid();
      changed = true;
   }
   else
   {
      changed         = false;
      first_populated = false;
   }
}
