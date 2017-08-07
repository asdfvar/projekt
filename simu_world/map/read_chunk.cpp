#include "read_chunk.h"
#include "fileio.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>






















#define maxPrimeIndex 10

static double Noise(int i, int x, int y)
{

   int primes[maxPrimeIndex][3] = {
      { 995615039, 600173719, 701464987 },
      { 831731269, 162318869, 136250887 },
      { 174329291, 946737083, 245679977 },
      { 362489573, 79591801, 350777237 },
      { 457025711, 880830799, 909678923 },
      { 787070341, 177340217, 593320781 },
      { 405493717, 291031019, 391950901 },
      { 458904767, 676625681, 424452397 },
      { 531736441, 939683957, 810651871 },
      { 997169939, 842027887, 423882827 }
   };

   int n = x + y * 57;
   n = (n << 13) ^ n;
   int a = primes[i][0], b = primes[i][1], c = primes[i][2];
   int t = (n * (n * n * a + b) + c) & 0x7fffffff;
   return 1.0 - (double)(t)/1073741824.0;
}

static double SmoothedNoise(int i, int x, int y)
{
   double corners = (Noise(i, x-1, y-1) + Noise(i, x+1, y-1) +
         Noise(i, x-1, y+1) + Noise(i, x+1, y+1)) / 16,
          sides = (Noise(i, x-1, y) + Noise(i, x+1, y) + Noise(i, x, y-1) +
                Noise(i, x, y+1)) / 8,
          center = Noise(i, x, y) / 4;
   return corners + sides + center;
}

static double Interpolate(double a, double b, double x) {  // cosine interpolation
   double ft = x * 3.1415927,
          f = (1 - cos(ft)) * 0.5;
   return  a*(1-f) + b*f;
}

static double InterpolatedNoise(int i, double x, double y)
{
   int integer_X = x;
   double fractional_X = x - integer_X;
   int integer_Y = y;
   double fractional_Y = y - integer_Y;

   double v1 = SmoothedNoise(i, integer_X, integer_Y),
          v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
          v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
          v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
          i1 = Interpolate(v1, v2, fractional_X),
          i2 = Interpolate(v3, v4, fractional_X);
   return Interpolate(i1, i2, fractional_Y);
}

static float ValueNoise_2D(double x, double y, int numOctaves, float persistence)
{
   double total = 0,
          frequency = pow(2, numOctaves),
          amplitude = 1;
   for (int i = 0; i < numOctaves; ++i)
   {
      frequency /= 2;
      amplitude *= persistence;
      total += InterpolatedNoise(i % maxPrimeIndex,
            x / frequency, y / frequency) * amplitude;
   }
   return total / frequency;
}


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
** function name: generate_chunk2
*/
void generate_chunk2( int* dst,
                      int  pos_x,
                      int  pos_y,
                      int  pos_z,
                      int  dim_x,
                      int  dim_y,
                      int  dim_z )
{
   std::srand( std::time(0) );

   for (int ind = 0, ind_z = 0; ind_z < dim_z; ind_z++)
   {
      for (int ind_y = 0; ind_y < dim_y; ind_y++)
      {

         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
            int x = pos_x + ind_x;
            int y = pos_y + ind_y;
            int z = pos_z + ind_z;

            float noise = ValueNoise_2D(x, y, 7, 0.5f);
            noise *= 100.0f;
            int height = (int)noise;
            if ( z <= height )
            {
               dst[ind] = 1;
            } else {
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
