#include "math_utils.h"
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iostream>

// Reduced Row-Echelon
void rref (float *matrix, int num_rows, int num_cols)
{
   const float eps = 0.000001f;
   for (int leading_element = 0; leading_element < num_rows; leading_element++)
   {
      float leading_element_value = matrix[leading_element * num_cols + leading_element];
      // Swap rows if the leading element is near zero
      for (int row = leading_element + 1;
            fabs (leading_element_value) < eps && row < num_rows;
            row++)
      {
         for (int col = 0; col < num_cols; col++)
         {
            float temp = matrix[leading_element * num_cols + col];
            matrix[leading_element * num_cols + col] = matrix[row * num_cols + col];
            matrix[row * num_cols + col] = temp;
         }
      }

      float leading_scalar = matrix[leading_element * num_cols + leading_element];

      for (int row = 0; row < num_rows; row++)
      {
         if (row == leading_element) continue;

         float row_factor = matrix[row * num_cols + leading_element];

         // Multiply and add to rows
         for (int col = 0; col < num_cols; col++)
         {
            matrix[row * num_cols + col] =
               leading_scalar * matrix[row             * num_cols + col] -
               row_factor     * matrix[leading_element * num_cols + col];
         }
      }
   }

   for (int leading_element = 0; leading_element < num_rows; leading_element++)
   {
      float leading_scalar = matrix[leading_element * num_cols + leading_element];

      // Normalize the row supporting the leading factor
      float leading_factor_inv = 1.0f / leading_scalar;

      matrix[leading_element * num_cols + leading_element] *= leading_factor_inv;

      for (int col = num_rows; col < num_cols; col++)
      {
         matrix[leading_element * num_cols + col] *= leading_factor_inv;
      }
   }
}

// Alternate variant of 2D Perlin noise using an arbitrary grid.
// The grid cells are randomly selected within the region
void alt_perlin (
      float *array,
      int    size[2],
      float  scale,
      int    num_grid_points,
      float  min_grid_point_dist,
      float *buffer)
{
   float *grid_cells[2];
   grid_cells[0] = (float*)buffer;
   grid_cells[1] = grid_cells[0]       + num_grid_points;

   float *gradient[2];
   gradient[0] = (float*)grid_cells[1] + num_grid_points;
   gradient[1] = gradient[0]           + num_grid_points;

   // Define the four corners to ensure the area is covered
   grid_cells[0][0] = -0.1f;
   grid_cells[0][1] = (float)(size[0] - 1) + 0.1f;
   grid_cells[0][2] = -0.1f;
   grid_cells[0][3] = (float)(size[0] - 1) + 0.1f;

   grid_cells[1][0] = -0.1f;
   grid_cells[1][1] = -0.1f;
   grid_cells[1][2] = (float)(size[1] - 1) + 0.1f;
   grid_cells[1][3] = (float)(size[1] - 1) + 0.1f;

#if 0
   for (int indx = 0, ind = 0; indx <= num_grid_points[0]; indx)
   {
      for (int indy = 0; indy <= num_grid_points[0]; indy, ind++)
      {
         grid_cells[0][ind] = indx * size[0] / num_grid_points[0];
         grid_cells[1][ind] = indy * size[1] / num_grid_points[1];
      }
   }
#endif

   for (int point = 4; point < num_grid_points; point++)
   {
      float dist2 = (float)(size[0] + size[1]);
      const float min_grid_point_dist2 = min_grid_point_dist * min_grid_point_dist;

      do {
         grid_cells[0][point] = (float)(rand () % 1000) / 1000.0f * (float)size[0];
         grid_cells[1][point] = (float)(rand () % 1000) / 1000.0f * (float)size[1];

         // min distance between all other existing points
         for (int l_point = 0; l_point < point; l_point++)
         {
            float grid_cell_dist2 =
               (grid_cells[0][l_point] - grid_cells[0][point]) * (grid_cells[0][l_point] - grid_cells[0][point]) +
               (grid_cells[1][l_point] - grid_cells[1][point]) * (grid_cells[1][l_point] - grid_cells[1][point]);

            if (grid_cell_dist2 < dist2) dist2 = grid_cell_dist2;
         }
      } while (dist2 < min_grid_point_dist2);

      // Random gradient value in scale * ([-1, 1) x [-1, 1)) range
      gradient[0][point] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
      gradient[1][point] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
   }

   // TODO: set this to the greatest distance between all grid cells
   float greatest_dist2 = 9999999.9f;

   for (int iy = 0, ind = 0; iy < size[1]; iy++)
   {
      for (int ix = 0; ix < size[0]; ix++, ind++)
      {
         float fix = (float)ix;
         float fiy = (float)iy;

         // Find the grid cell closest to this cell and is <= this cell in x and y
         float grid_cell_ll[2] = { grid_cells[0][0], grid_cells[1][0] };
         float gradient_ll[2] = { gradient[0][0], gradient[1][0] };

         float min_dist2 = greatest_dist2;

         for (int cell = 0; cell < num_grid_points; cell++)
         {
            if (grid_cells[0][cell] <= fix && grid_cells[1][cell] <= fiy)
            {
               float dist2 =
                  (grid_cells[0][cell] - fix) * (grid_cells[0][cell] - fix) +
                  (grid_cells[1][cell] - fiy) + (grid_cells[1][cell] - fiy);

               if (dist2 < min_dist2)
               {
                  min_dist2 = dist2;
                  grid_cell_ll[0] = grid_cells[0][cell];
                  grid_cell_ll[1] = grid_cells[1][cell];
                  gradient_ll[0] = gradient[0][cell];
                  gradient_ll[1] = gradient[1][cell];
               }
            }
         }

         // Find the grid cell closest to this cell and is > this cell in x and <= this cell in y
         float grid_cell_lr[2] = { grid_cells[0][1], grid_cells[1][1] };
         float gradient_lr[2] = { gradient[0][1], gradient[1][1] };

         min_dist2 = greatest_dist2;

         for (int cell = 0; cell < num_grid_points; cell++)
         {
            if (grid_cells[0][cell] > fix && grid_cells[1][cell] <= fiy)
            {
               float dist2 =
                  (grid_cells[0][cell] - fix) * (grid_cells[0][cell] - fix) +
                  (grid_cells[1][cell] - fiy) + (grid_cells[1][cell] - fiy);

               if (dist2 < min_dist2)
               {
                  min_dist2 = dist2;
                  grid_cell_lr[0] = grid_cells[0][cell];
                  grid_cell_lr[1] = grid_cells[1][cell];
                  gradient_lr[0] = gradient[0][cell];
                  gradient_lr[1] = gradient[1][cell];
               }
            }
         }

         // Find the grid cell closest to this cell and is <= this cell in x and > this cell in y
         float grid_cell_ul[2] = { grid_cells[0][2], grid_cells[1][2] };
         float gradient_ul[2] = { gradient[0][2], gradient[1][2] };

         min_dist2 = greatest_dist2;

         for (int cell = 0; cell < num_grid_points; cell++)
         {
            if (grid_cells[0][cell] <= fix && grid_cells[1][cell] > fiy)
            {
               float dist2 =
                  (grid_cells[0][cell] - fix) * (grid_cells[0][cell] - fix) +
                  (grid_cells[1][cell] - fiy) + (grid_cells[1][cell] - fiy);

               if (dist2 < min_dist2)
               {
                  min_dist2 = dist2;
                  grid_cell_ul[0] = grid_cells[0][cell];
                  grid_cell_ul[1] = grid_cells[1][cell];
                  gradient_ul[0] = gradient[0][cell];
                  gradient_ul[1] = gradient[1][cell];
               }
            }
         }

         // Find the grid cell closest to this cell and is > this cell in x and > this cell in y
         float grid_cell_ur[2] = { grid_cells[0][3], grid_cells[1][3] };
         float gradient_ur[2] = { gradient[0][3], gradient[1][3] };

         min_dist2 = greatest_dist2;

         for (int cell = 0; cell < num_grid_points; cell++)
         {
            if (grid_cells[0][cell] > fix && grid_cells[1][cell] > fiy)
            {
               float dist2 =
                  (grid_cells[0][cell] - fix) * (grid_cells[0][cell] - fix) +
                  (grid_cells[1][cell] - fiy) + (grid_cells[1][cell] - fiy);

               if (dist2 < min_dist2)
               {
                  min_dist2 = dist2;
                  grid_cell_ur[0] = grid_cells[0][cell];
                  grid_cell_ur[1] = grid_cells[1][cell];
                  gradient_ur[0] = gradient[0][cell];
                  gradient_ur[1] = gradient[1][cell];
               }
            }
         }

         float dotp_ll =
            (fix - grid_cell_ll[0]) * gradient_ll[0] +
            (fiy - grid_cell_ll[1]) * gradient_ll[1];

         float dotp_lr =
            (fix - grid_cell_lr[0]) * gradient_lr[0] +
            (fiy - grid_cell_lr[1]) * gradient_lr[1];

         float dotp_ul =
            (fix - grid_cell_ul[0]) * gradient_ul[0] +
            (fiy - grid_cell_ul[1]) * gradient_ul[1];

         float dotp_ur =
            (fix - grid_cell_ur[0]) * gradient_ur[0] +
            (fiy - grid_cell_ur[1]) * gradient_ur[1];

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
         //
         // TODO: ensure stability by having each point be at least some minimum distance from all other points

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

         // Reduced Row-Echelon form
         rref (matrix, 4, 5);

         float A = matrix[4];
         float B = matrix[9];
         float C = matrix[14];
         float D = matrix[19];

         array[ind] = A * fix * fiy + B * fix + C * fiy + D;
      }
   }

}
