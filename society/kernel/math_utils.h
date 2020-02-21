#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Compute the reduced row-Echelon form of the matrix
void rref (float *matrix, int num_rows, int num_cols);

// Alternate variant of 2D Perlin noise.
// The grid cells are randomly selected within the region defined by the size.
void alt_perlin (
      float *array, // array to receive the result
      int    size[2],
      float  scale,
      int    num_grid_points,
      float  min_grid_point_dist,
      float *buffer);

#endif
