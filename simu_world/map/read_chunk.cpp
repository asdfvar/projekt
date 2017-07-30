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
bool read_chunk( std::string& filename,
                 int*         chunk_elements,
                 int          num_chunk_elements )
{
   return fio::read( filename,            // path and file name included
                   0,                     // offset in bytes from the beginning of file
                   (void*)chunk_elements, // buffer to hold the data
                   num_chunk_elements * sizeof(*chunk_elements));
}
