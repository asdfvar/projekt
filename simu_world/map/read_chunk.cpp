#include "read_chunk.h"
#include "fileio.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

void create_random( int* blocks, int  num_chunk_elements )
{
   for (int ind = 0; ind < num_chunk_elements; ind++)
   {
      int random_num = std::rand() % 100;
      if (random_num == 0)
         blocks[ind] = 1;
      else
         blocks[ind] = 0;
   }
}


/*
** function name: read_chunk
*/
void read_chunk( int* chunk_elements,
                 int  ind_x,
                 int  ind_y,
                 int  ind_z,
                 int  num_chunk_elements )
{
#ifdef __linux__
   std::string chunk_name = "saves/chunk_";
#elif _WIN32
   std::string chunk_name = "saves\chunk_";
#else
   std::cout << __FILE__ << ":" << __LINE__ << ": unknown OS" << std::endl;
#endif

   std::ostringstream id_str;
   if (ind_x < 0)
   {
      id_str << -ind_x;
      chunk_name += "n";
   }
   else
   {
      id_str << ind_x;
   }
   chunk_name += id_str.str();
   chunk_name += "_";

   id_str.clear();
   id_str.str("");
   if (ind_y < 0)
   {
      id_str << -ind_y;
      chunk_name += "n";
   }
   else
   {
      id_str << ind_y;
   }
   chunk_name += id_str.str();
   chunk_name += "_";

   id_str.clear();
   id_str.str("");
   if (ind_z < 0)
   {
      id_str << -ind_z;
      chunk_name += "n";
   }
   else
   {
      id_str << ind_z;
   }
   chunk_name += id_str.str();

  bool file_exists =
         fio::read( chunk_name,           // path and file name included
                   0,                     // offset in bytes from the beginning of file
                   (void*)chunk_elements, // buffer to hold the data
                   num_chunk_elements * sizeof(*chunk_elements));

   if( !file_exists ) // number of bytes to read
   {
      create_random( chunk_elements,
                     num_chunk_elements );
   }

   std::cout << "reading in chunk from (" << ind_x
             << ", " << ind_y << ", " << ind_z << ")"
             << std::endl;
}
