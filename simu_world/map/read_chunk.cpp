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

#define PREC 10000
/*
** function name: generate_chunk
*/
void generate_chunk2( int* src,
                      int* dst,
                      int  pos_x,
                      int  pos_y,
                      int  pos_z,
                      int  src_dim_x,
                      int  src_dim_y,
                      int  src_dim_z,
                      int  dst_dim_x,
                      int  dst_dim_y,
                      int  dst_dim_z )
{
   std::srand( std::time(0) );

   int aug = (dst_dim_x - src_dim_x) / 2;

   int total_dst_dim = dst_dim_x * dst_dim_y * dst_dim_z;

   for (int cycle = 0; cycle < 10000; cycle++)
   {

      int ind = std::rand() % total_dst_dim;
      int ind_x = ind % dst_dim_x;
      int ind_y = (ind / dst_dim_x) % dst_dim_z;
      int ind_z = ind / (dst_dim_x * dst_dim_y);

      // get the equivalent source index for the destination index
      int src_ind = src_dim_x * src_dim_y * (aug + ind_z) +
                    src_dim_x * (aug + ind_y) +
                    aug + ind_x;

      if (dst[ind] == 0)
      {
         if ((src[src_ind - 1] != 0)         ||
               (src[src_ind + 1] != 0)         ||
               (src[src_ind - src_dim_x] != 0) ||
               (src[src_ind + src_dim_x] != 0))
         {
            // generate a random number between [0, 1)
            int random_int     = std::rand() % PREC;
            float random_float = (float)random_int;
            random_float      /= (float)PREC;

            if (random_float < 0.95f) dst[ind] = 1;
            else dst[ind] = 0;
         }
         else if (src[src_ind - src_dim_x * src_dim_y] != 0)
         {
            // generate a random number between [0, 1)
            int random_int     = std::rand() % PREC;
            float random_float = (float)random_int;
            random_float      /= (float)PREC;

            if (random_float < 0.05f) dst[ind] = 1;
            else dst[ind] = 0;
         }
         else if (src[src_ind + src_dim_x * src_dim_y] != 0)
         {
            // generate a random number between [0, 1)
            int random_int     = std::rand() % PREC;
            float random_float = (float)random_int;
            random_float      /= (float)PREC;

            if (random_float < 1.0f) dst[ind] = 1;
            else dst[ind] = 0;
         }
         else dst[ind] = 0;
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
