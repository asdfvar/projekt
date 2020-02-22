#include "math_utils.h"
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iostream>

// Reduced Row-Echelon
void rref (float *matrix, int num_rows, int num_cols)
{
   for (int leading_element = 0; leading_element < num_rows; leading_element++)
   {
      float leading_element_value = matrix[leading_element * num_cols + leading_element];

      // Swap rows if the leading element is near zero
      for (int row = leading_element + 1; row < num_rows; row++)
      {
         if ( matrix[row * num_cols + leading_element] > matrix[leading_element * num_cols + leading_element])
            for (int col = leading_element; col < num_cols; col++)
            {
               float temp = matrix[leading_element * num_cols + col];
               matrix[leading_element * num_cols + col] = matrix[row * num_cols + col];
               matrix[row * num_cols + col] = temp;
            }

         leading_element_value = matrix[leading_element * num_cols + leading_element];
      }

// <<< left off here (stability fix)
      leading_element_value = matrix[leading_element * num_cols + leading_element];

      for (int col = leading_element; col < num_cols; col++)
      {
         matrix[leading_element * num_cols + col] /= leading_element_value;
      }

      leading_element_value = matrix[leading_element * num_cols + leading_element];
// >>> left off here (stability fix)

      for (int row = 0; row < num_rows; row++)
      {
         if (row == leading_element) continue;

         float row_factor = matrix[row * num_cols + leading_element];

         // Multiply and add to rows
         for (int col = 0; col < num_cols; col++)
         {
            matrix[row * num_cols + col] =
               leading_element_value * matrix[row             * num_cols + col] -
               row_factor            * matrix[leading_element * num_cols + col];
         }
      }
   }

   for (int leading_element = 0; leading_element < num_rows; leading_element++)
   {
      float leading_element_value = matrix[leading_element * num_cols + leading_element];

      // Normalize the row supporting the leading factor
      float leading_factor_inv = 1.0f / leading_element_value;

      matrix[leading_element * num_cols + leading_element] *= leading_factor_inv;

      for (int col = num_rows; col < num_cols; col++)
      {
         matrix[leading_element * num_cols + col] *= leading_factor_inv;
      }
   }
}

// Alternate variant of 2D Perlin noise using an arbitrary grid.
// The grid cells are randomly selected within the region
void perlin (
      float *array,
      int    size[2],
      float  scale,
      int    num_grid_cells[2],
      float  min_grid_point_dist,
      float *buffer)
{
   int *grid_cells[2];
   float *gradient[2];
   grid_cells[0] = (int*)buffer;
   grid_cells[1] = grid_cells[0]         + (num_grid_cells[0] + 1) * (num_grid_cells[1] + 1);
   gradient[0]   = (float*)grid_cells[1] + (num_grid_cells[0] + 1) * (num_grid_cells[1] + 1);
   gradient[1]   = gradient[0]           + (num_grid_cells[0] + 1) * (num_grid_cells[1] + 1);

   for (int indy = 0, ind = 0; indy <= num_grid_cells[0]; indy++)
   {
      for (int indx = 0; indx <= num_grid_cells[0]; indx++, ind++)
      {
         grid_cells[0][ind] = indx * size[0] / num_grid_cells[0];
         grid_cells[1][ind] = indy * size[1] / num_grid_cells[1];

         gradient[0][ind] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
         gradient[1][ind] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
      }
   }

   int step[2] = {
      size[0] / num_grid_cells[0],
      size[1] / num_grid_cells[1] };

   for (int iy = 0, ind = 0; iy < size[1]; iy++)
   {
      for (int ix = 0; ix < size[0]; ix++, ind++)
      {
         int grid_ind_ll = (iy / step[1]    ) * (num_grid_cells[0] + 1) + (ix / step[0]);
         int grid_ind_lr = (iy / step[1]    ) * (num_grid_cells[0] + 1) + (ix / step[0]) + 1;
         int grid_ind_ul = (iy / step[1] + 1) * (num_grid_cells[0] + 1) + (ix / step[0]);
         int grid_ind_ur = (iy / step[1] + 1) * (num_grid_cells[0] + 1) + (ix / step[0]) + 1;

         int   grid_cell_ll[2] = { grid_cells[0][grid_ind_ll], grid_cells[1][grid_ind_ll] };
         float gradient_ll[2]  = { gradient  [0][grid_ind_ll], gradient  [1][grid_ind_ll] };

         int   grid_cell_lr[2] = { grid_cells[0][grid_ind_lr], grid_cells[1][grid_ind_lr] };
         float gradient_lr[2]  = { gradient  [0][grid_ind_lr], gradient  [1][grid_ind_lr] };

         int   grid_cell_ul[2] = { grid_cells[0][grid_ind_ul], grid_cells[1][grid_ind_ul] };
         float gradient_ul[2]  = { gradient  [0][grid_ind_ul], gradient  [1][grid_ind_ul] };

         int   grid_cell_ur[2] = { grid_cells[0][grid_ind_ur], grid_cells[1][grid_ind_ur] };
         float gradient_ur[2]  = { gradient  [0][grid_ind_ur], gradient  [1][grid_ind_ur] };

         float dotp_ll =
            (float)(ix - grid_cell_ll[0]) * gradient_ll[0] +
            (float)(iy - grid_cell_ll[1]) * gradient_ll[1];

         float dotp_lr =
            (float)(ix - grid_cell_lr[0]) * gradient_lr[0] +
            (float)(iy - grid_cell_lr[1]) * gradient_lr[1];

         float dotp_ul =
            (float)(ix - grid_cell_ul[0]) * gradient_ul[0] +
            (float)(iy - grid_cell_ul[1]) * gradient_ul[1];

         float dotp_ur =
            (float)(ix - grid_cell_ur[0]) * gradient_ur[0] +
            (float)(iy - grid_cell_ur[1]) * gradient_ur[1];

         // 2D bilinear interpolation on an arbitrary grid can be
         // obtained by solving for the parameters A, B, C, D s.t.
         //
         // w(x,y) = A*x*y + B*x + C*y + D
         //
         // and
         // w(ll) = dotp_ll
         // w(lr) = dotp_lr
         // w(ul) = dotp_ul
         // w(ur) = dotp_ur
         //
         // so that
         //
         // dotp_ll = A * ll_0 * ll_1 + B * ll_0 + C * ll_1 + D
         // dotp_lr = A * lr_0 * lr_1 + B * lr_0 + C * lr_1 + D
         // dotp_ur = A * ur_0 * ur_1 + B * ur_0 + C * ur_1 + D
         // dotp_ul = A * ul_0 * ul_1 + B * ul_0 + C * ul_1 + D

         float ll_0 = (float)grid_cell_ll[0];
         float ll_1 = (float)grid_cell_ll[1];
         float lr_0 = (float)grid_cell_lr[0];
         float lr_1 = (float)grid_cell_lr[1];
         float ur_0 = (float)grid_cell_ur[0];
         float ur_1 = (float)grid_cell_ur[1];
         float ul_0 = (float)grid_cell_ul[0];
         float ul_1 = (float)grid_cell_ul[1];

         float matrix[4 * 5] = {
            ll_0 * ll_1, ll_0, ll_1, 1.0f, dotp_ll,
            lr_0 * lr_1, lr_0, lr_1, 1.0f, dotp_lr,
            ur_0 * ur_1, ur_0, ur_1, 1.0f, dotp_ur,
            ul_0 * ul_1, ul_0, ul_1, 1.0f, dotp_ul };

#if 1
   std::cout << "matrix before RREF:" << std::endl;
   for (int row = 0, ind = 0; row < 4; row++)
   {
      for (int col = 0; col < 5; col++, ind++)
         std::cout << matrix[ind] << ", ";
      std::cout << std::endl;
   }
   std::cout << std::endl;
#endif
         // Reduced Row-Echelon form
         rref (matrix, 4, 5);
#if 1
   std::cout << "matrix after RREF:" << std::endl;
   for (int row = 0, ind = 0; row < 4; row++)
   {
      for (int col = 0; col < 5; col++, ind++)
         std::cout << matrix[ind] << ", ";
      std::cout << std::endl;
   }
   std::cout << std::endl;
#endif

         float A = matrix[4];
         float B = matrix[9];
         float C = matrix[14];
         float D = matrix[19];

         array[ind] = A * (float)ix * (float)iy + B * (float)ix + C * (float)iy + D;
      }
   }

}
