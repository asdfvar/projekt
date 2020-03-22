#ifndef UTILS_H
#define UTILS_H

/*
 * Simple utility routines
 */

// Convert a flattened index to the specified Cartesian index
static int flat_ind_to_dim (int dim, int flat_ind, int *size)
{

   int prod = 1;

   for (int ind = 0; ind < dim; ind++)
      prod *= size[ind];

   int ret = flat_ind / prod;

   ret %= size[dim];

   return ret;
}

// Convert a flattened index to the specified Cartesian index
static int dim_to_flat_ind (int dim, int *ind, int *size)
{
   int prod = 1;

   int ret = ind[0];

   for (int flat_ind = 1; flat_ind < dim; flat_ind++)
   {
      prod *= size[flat_ind - 1];
      ret  += ind[flat_ind] * prod;
   }

   return ret;
}

// Convert an *x* map point to an *x* window point in a single dimension
static float map_to_window (float position, float map_size)
{
   return position / map_size * 2.0f - 1.0f;
}

// Apply 2-D transform with translation
static inline void transformation (float point[2], float transform[4], float translation[2])
{
   point[0] += translation[0];
   point[1] += translation[1];

   float part1a = point[0] * transform[0];
   float part1b = point[1] * transform[1];

   float part2a = point[0] * transform[2];
   float part2b = point[1] * transform[3];

   point[0] = part1a + part1b;
   point[1] = part2a + part2b;
}

#endif
