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

#endif
