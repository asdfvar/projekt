#include "graphics.h"
#include <iostream>
#include <cmath>

#define X_START -0.8f
#define Y_START -0.8f

#define WIDTH   0.8f
#define HEIGHT  0.8f

void draw_map (const float *map, int map_dims[3], int map_layer)
{
   int row_max = map_dims[1];
   int col_max = map_dims[0];

   int window_width  = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   float starting_row_loc = Y_START;
   float starting_col_loc = X_START;

   for (int row = 0, ind = map_layer * row_max * col_max; row < row_max; row++)
   {
      for (int col = 0; col < col_max; col++, ind++)
      {
         float vertex_x = starting_col_loc + block_width  * (float)col;
         float vertex_y = starting_row_loc + block_height * (float)row;

         glBegin (GL_POLYGON);
         if (map[ind] < 0.0f) {
            glColor3f (0.0f, 0.0f, 0.0f);
         } else {
            glColor3f (1.0f, 1.0f, 1.0f);
         }
         glVertex2f (vertex_x,               vertex_y               );
         glVertex2f (vertex_x + block_width, vertex_y               );
         glVertex2f (vertex_x + block_width, vertex_y + block_height);
         glVertex2f (vertex_x,               vertex_y + block_height);
         glEnd ();
      }
   }
}

void draw_units (float *x, float *y, float *z, int map_dims[3], int num_units, int map_layer)
{
   int row_max = map_dims[1];
   int col_max = map_dims[0];

   int window_width  = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   float window_start_row = -0.8f;
   float window_end_row   =  0.8f;
   float window_start_col = -0.8f;
   float window_end_col   =  0.8f;

   for (int ind = 0; ind < num_units; ind++)
   {
      if (floorf (z[ind]) == map_layer) {

      float x_pos = x[ind] / ((float)col_max) *
                    (window_end_col - window_start_col) +
                    window_start_col;

      float y_pos = y[ind] / ((float)row_max) *
                    (window_end_row - window_start_row) +
                    window_start_row;

         glBegin (GL_POLYGON);
         glColor3f (0.0f, 1.0f, 0.0f);
         glVertex2f (x_pos - 0.01f, y_pos - 0.01f);
         glVertex2f (x_pos - 0.01f, y_pos + 0.01f);
         glVertex2f (x_pos + 0.01f, y_pos + 0.01f);
         glVertex2f (x_pos + 0.01f, y_pos - 0.01f);
         glEnd ();
      }
   }
}
