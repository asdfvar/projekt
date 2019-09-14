#include "graphics.h"

void draw_map (const float *map, int map_dims[3], int map_layer)
{
   int row_max = map_dims[1];
   int col_max = map_dims[0];

   int window_width = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

#if 0
glBegin (GL_POLYGON);
// glColor3f (color0, color1, color2)
glVertex2f (-0.5, -0.5);
glVertex2f (-0.5, 0.5);
glVertex2f (0, 0.5);
glVertex2f (0, -0.5);
glEnd ();
#endif

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
#if 1
         if (map[ind] < 0.0f) {
            glColor3f (0.0f, 0.0f, 0.0f);
         } else {
            glColor3f (1.0f, 1.0f, 1.0f);
         }
#endif
         glVertex2f (vertex_x,               vertex_y               );
         glVertex2f (vertex_x + block_width, vertex_y               );
         glVertex2f (vertex_x + block_width, vertex_y + block_height);
         glVertex2f (vertex_x,               vertex_y + block_height);
         glEnd ();
      }
   }
}
