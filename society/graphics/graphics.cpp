#include "graphics.h"
#include <iostream>
#include <cmath>

void draw_map (const float *map, int map_dims[3], int map_layer)
{
   int row_max = map_dims[1];
   int col_max = map_dims[0];

   int window_width = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   float block_height = 0.8f / static_cast<float>(row_max / 2);
   float block_width  = 0.8f / static_cast<float>(col_max / 2);

   float starting_row_loc = -0.8f;
   float starting_col_loc = -0.8f;

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

void draw_units (float *x, float *y, float *z, int num_units, int map_layer)
{
   for (int ind = 0; ind < num_units; ind++)
   {
      if (floorf (z[ind]) == map_layer) {
         glBegin (GL_POLYGON);
         glColor3f (0.0f, 1.0f, 0.0f);
         glVertex2f (x[ind] - 0.01f, y[ind] - 0.01f);
         glVertex2f (x[ind] - 0.01f, y[ind] + 0.01f);
         glVertex2f (x[ind] + 0.01f, y[ind] + 0.01f);
         glVertex2f (x[ind] + 0.01f, y[ind] - 0.01f);
         glEnd ();
      }
   }
}
