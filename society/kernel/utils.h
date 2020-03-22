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

// Convert an *x* map point to an *x* window point in a single dimension
static float map_to_window (float position, float map_size)
{
   return position / map_size * 2.0f - 1.0f;
}

#endif
