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
         if (matrix[row * num_cols + leading_element] > matrix[leading_element * num_cols + leading_element])
            for (int col = leading_element; col < num_cols; col++)
            {
               float temp = matrix[leading_element * num_cols + col];
               matrix[leading_element * num_cols + col] = matrix[row * num_cols + col];
               matrix[row * num_cols + col] = temp;
            }

         leading_element_value = matrix[leading_element * num_cols + leading_element];
      }

      float leading_element_value_inv = 1.0f / matrix[leading_element * num_cols + leading_element];

      for (int col = leading_element; col < num_cols; col++)
         matrix[leading_element * num_cols + col] *= leading_element_value_inv;

      for (int row = 0; row < num_rows; row++)
      {
         if (row == leading_element) continue;

         float row_factor = matrix[row * num_cols + leading_element];

         // Multiply and add to rows
         for (int col = 0; col < num_cols; col++)
         {
            matrix[row * num_cols + col] =
               matrix[row * num_cols + col] -
               row_factor * matrix[leading_element * num_cols + col];
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
      int    num_grid_cells[2])
{
   float *top_gradients[2];
   top_gradients[0]    = new float[num_grid_cells[0] + 1];
   top_gradients[1]    = new float[num_grid_cells[0] + 1];

   float *bottom_gradients[2];
   bottom_gradients[0] = new float[num_grid_cells[0] + 1];
   bottom_gradients[1] = new float[num_grid_cells[0] + 1];

   for (int indx = 0; indx <= num_grid_cells[0]; indx++)
   {
      top_gradients[0][indx]    = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
      top_gradients[1][indx]    = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
      bottom_gradients[0][indx] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
      bottom_gradients[1][indx] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
   }

   int step[2] = { size[0] / num_grid_cells[0], size[1] / num_grid_cells[1] };

   for (int iy = 0, ind = 0; iy < size[1]; iy++)
   {
      for (int ix = 0; ix < size[0]; ix++, ind++)
      {
         int cell[2] = { ix / step[0], iy / step[1] };

         float center[2] = { 0.5f * step[0], 0.5f * step[1] };

         float dotp_ll =
            (float)(ix - cell[0] * step[0])       * bottom_gradients[0][cell[0]] +
            (float)(iy - cell[1] * step[1])       * bottom_gradients[1][cell[0]];

dotp_ll = bottom_gradients[0][cell[0]];

         float dotp_lr =
            (float)(ix - (cell[0] + 1) * step[0]) * bottom_gradients[0][cell[0]+1] +
            (float)(iy - cell[1] * step[1])       * bottom_gradients[1][cell[0]];

dotp_lr = bottom_gradients[0][cell[0]+1];

         float dotp_ul =
            (float)(ix - cell[0] * step[0])       * top_gradients[0][cell[0]] +
            (float)(iy - (cell[1] + 1) * step[1]) * top_gradients[1][cell[0]+1];

dotp_ul = top_gradients[0][cell[0]];

         float dotp_ur =
            (float)(ix - (cell[0] + 1) * step[0]) * top_gradients[0][cell[0]+1] +
            (float)(iy - (cell[1] + 1) * step[1]) * top_gradients[1][cell[0]+1];

dotp_ur = top_gradients[0][cell[0]+1];

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

         float ll_0 = 0.0f;
         float ll_1 = 0.0f;
         float lr_0 = step[0];
         float lr_1 = 0.0f;
         float ur_0 = step[0];
         float ur_1 = step[1];
         float ul_0 = 0.0f;
         float ul_1 = step[1];

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

         // map the fine cell to the equivalent fine cell in the coarse cell
         float fix = (float)(ix - cell[0] * step[0]);
         float fiy = (float)(iy - cell[1] * step[1]);

         array[ind] += A * fix * fiy + B * fix + C * fiy + D;
      }

      if ((iy + 1) % step[1] == 0)
      {
         // Rotate the gradient buffers (push the top gradients to the bottom gradients)
         for (int indx = 0; indx <= num_grid_cells[0]; indx++)
         {
            float *temp[2];

            temp[0]             = bottom_gradients[0];
            temp[1]             = bottom_gradients[1];
            bottom_gradients[0] = top_gradients[0];
            bottom_gradients[1] = top_gradients[1];
            top_gradients[0]    = temp[0];
            top_gradients[1]    = temp[1];
         }

         // Compute new top gradients
         for (int indx = 0; indx <= num_grid_cells[0]; indx++)
         {
            top_gradients[0][indx] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
            top_gradients[1][indx] = ((float)(rand () % 2000) / 1000.0f - 1.0f) * scale;
         }
      }

   }

   delete[] top_gradients[0];
   delete[] top_gradients[1];
   delete[] bottom_gradients[0];
   delete[] bottom_gradients[1];
}
