#include "read_chunk.h"
#include "fileio.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

/*
** function name: create_random
*/
void create_random( int* blocks, int num_chunk_elements )
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
** function name: generate_chunk
*/
void generate_chunk( int* dst,
                     int  pos_x,
                     int  pos_y,
                     int  pos_z,
                     int  dim_x,
                     int  dim_y,
                     int  dim_z )
{
   std::srand(std::time(0));
   for (int ind = 0, ind_z = 0; ind_z < dim_z; ind_z++)
   {
      for (int ind_y = 0; ind_y < dim_y; ind_y++)
      {
         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
            int block_pos_z = pos_z + ind_z;

            float f_block_pos_z = (float)block_pos_z;
            float beta          = 0.3f;
            float sigmoid       = 1.0f / (1.0f + exp( -f_block_pos_z * beta ));

            float result = sigmoid * 110.0f;
            int   random_num = 1 + (int)result;

            if (random_num > 100) dst[ind] = 0;
            else
            {
               int number = std::rand() % random_num;
               if (number == 0)
                  dst[ind] = 1;
               else
                  dst[ind] = 0;
            }
         }
      }
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
