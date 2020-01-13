#include "society.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <cmath>

#define X_START -1.0f
#define Y_START -1.0f

#define WIDTH   1.0f
#define HEIGHT  1.0f

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


void Society::draw_units (
      float *transform,
      float *translation,
      int    map_layer)
{
   int row_max = dim[1];
   int col_max = dim[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float window_start_row = -1.0f;
   const float window_end_row   =  1.0f;
   const float window_start_col = -1.0f;
   const float window_end_col   =  1.0f;

   const float width_offset  = 0.3f * block_width;
   const float height_offset = 0.3f * block_height;

   const float selected_width_offset  = 0.35f * block_width;
   const float selected_height_offset = 0.35f * block_height;

   glLineWidth (0.1f);
   for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); unit++)
   {
      if (floorf ((*unit)->get_position (2)) == map_layer)
      {
         float x_pos = (*unit)->get_position (0) / ((float)col_max) *
            (window_end_col - window_start_col) +
            window_start_col;

         float y_pos = (*unit)->get_position (1) / ((float)row_max) *
            (window_end_row - window_start_row) +
            window_start_row;

         float point0[2] = { x_pos - width_offset, y_pos - height_offset };
         float point1[2] = { x_pos - width_offset, y_pos + height_offset };
         float point2[2] = { x_pos + width_offset, y_pos + height_offset };
         float point3[2] = { x_pos + width_offset, y_pos - height_offset };

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glBegin (GL_POLYGON);
         glColor3f (0.0f, 1.0f, 0.0f);
         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glEnd ();

         if ((*unit)->is_selected())
         {
            float point0[2] = { x_pos - selected_width_offset, y_pos - selected_height_offset };
            float point1[2] = { x_pos - selected_width_offset, y_pos + selected_height_offset };
            float point2[2] = { x_pos + selected_width_offset, y_pos + selected_height_offset };
            float point3[2] = { x_pos + selected_width_offset, y_pos - selected_height_offset };

            transformation (point0, transform, translation);
            transformation (point1, transform, translation);
            transformation (point2, transform, translation);
            transformation (point3, transform, translation);

            glBegin (GL_LINES);
            glColor3f (1.0f, 0.0f, 0.0f);
            glVertex2f (point0[0], point0[1]);
            glVertex2f (point1[0], point1[1]);
            glVertex2f (point1[0], point1[1]);
            glVertex2f (point2[0], point2[1]);
            glVertex2f (point2[0], point2[1]);
            glVertex2f (point3[0], point3[1]);
            glVertex2f (point3[0], point3[1]);
            glVertex2f (point0[0], point0[1]);
            glEnd ();
         }
      }
   }
}
