#include "graphics.h"
#include <iostream>
#include <cmath>

#define X_START -1.0f
#define Y_START -1.0f

#define WIDTH   1.0f
#define HEIGHT  1.0f

void draw_map (
      float       *transform,
      float       *translation,
      const float *map,
      int          map_dims[3],
      int          map_layer)
{
   int row_max = map_dims[1];
   int col_max = map_dims[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   float starting_row_loc = Y_START;
   float starting_col_loc = X_START;

   const float det = transform[0] * transform[3] - transform[1] * transform[2];
   const float invDet = 1.0f / det;

   const float inv_transform[4] =
   { invDet * transform[3], -invDet * transform[1],
      -invDet * transform[2],  invDet * transform[0] };

   float base_translation[2];
   float temp = base_translation[0];
   base_translation[0] = translation[0] * inv_transform[0] + translation[1] * inv_transform[1];
   base_translation[1] = translation[0] * inv_transform[2] + translation[1] * inv_transform[3];

   // draw grid lines
   for (int row = 0, ind = map_layer * row_max * col_max; row < row_max; row++)
   {
      for (int col = 0; col < col_max; col++, ind++)
      {
         float vertex_x = starting_col_loc + block_width  * (float)col;
         float vertex_y = starting_row_loc + block_height * (float)row;

         float point0[2] = { vertex_x,               vertex_y               };
         float point1[2] = { vertex_x + block_width, vertex_y               };
         float point2[2] = { vertex_x + block_width, vertex_y + block_width };
         float point3[2] = { vertex_x,               vertex_y + block_width };

         point0[0] += translation[0];
         point0[1] += translation[1];

         float temp;
         temp = point0[0];
         point0[0] = point0[0] * transform[0] + point0[1] * transform[1];// + translation[0];
         point0[1] = temp      * transform[2] + point0[1] * transform[3];// + translation[1];

         point1[0] += translation[0];
         point1[1] += translation[1];

         temp = point1[0];
         point1[0] = point1[0] * transform[0] + point1[1] * transform[1];// + translation[0];
         point1[1] = temp      * transform[2] + point1[1] * transform[3];// + translation[1];

         point2[0] += translation[0];
         point2[1] += translation[1];

         temp = point2[0];
         point2[0] = point2[0] * transform[0] + point2[1] * transform[1];// + translation[0];
         point2[1] = temp      * transform[2] + point2[1] * transform[3];// + translation[1];

         point3[0] += translation[0];
         point3[1] += translation[1];

         temp = point3[0];
         point3[0] = point3[0] * transform[0] + point3[1] * transform[1];// + translation[0];
         point3[1] = temp      * transform[2] + point3[1] * transform[3];// + translation[1];

         glBegin (GL_LINES);
         glColor3f (0.0f, 0.0f, 1.0f);
         glVertex2f (point0[0], point0[1] );
         glVertex2f (point1[0], point1[1] );
         glVertex2f (point1[0], point1[1] );
         glVertex2f (point2[0], point2[1] );
         glVertex2f (point2[0], point2[1] );
         glVertex2f (point3[0], point3[1] );
         glVertex2f (point3[0], point3[1] );
         glVertex2f (point0[0], point0[1] );
         glEnd();
      }
   }

   // draw grid cells
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

         float point0[2] = { vertex_x,               vertex_y               };
         float point1[2] = { vertex_x + block_width, vertex_y               };
         float point2[2] = { vertex_x + block_width, vertex_y + block_width };
         float point3[2] = { vertex_x,               vertex_y + block_width };

         point0[0] += translation[0];
         point0[1] += translation[1];

         float temp;
         temp = point0[0];
         point0[0] = point0[0] * transform[0] + point0[1] * transform[1];// + translation[0];
         point0[1] = temp      * transform[2] + point0[1] * transform[3];// + translation[1];

         point1[0] += translation[0];
         point1[1] += translation[1];

         temp = point1[0];
         point1[0] = point1[0] * transform[0] + point1[1] * transform[1];// + translation[0];
         point1[1] = temp      * transform[2] + point1[1] * transform[3];// + translation[1];

         point2[0] += translation[0];
         point2[1] += translation[1];

         temp = point2[0];
         point2[0] = point2[0] * transform[0] + point2[1] * transform[1];// + translation[0];
         point2[1] = temp      * transform[2] + point2[1] * transform[3];// + translation[1];

         point3[0] += translation[0];
         point3[1] += translation[1];

         temp = point3[0];
         point3[0] = point3[0] * transform[0] + point3[1] * transform[1];// + translation[0];
         point3[1] = temp      * transform[2] + point3[1] * transform[3];// + translation[1];

         glVertex2f (point0[0], point0[1] );
         glVertex2f (point1[0], point1[1] );
         glVertex2f (point2[0], point2[1] );
         glVertex2f (point3[0], point3[1] );
         glEnd ();
      }
   }
}

void draw_units (
      float *transform,
      float *translation,
      float *x,
      float *y,
      float *z,
      bool  *selected,
      int    map_dims[3],
      int    num_units,
      int    map_layer)
{
   int row_max = map_dims[1];
   int col_max = map_dims[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float window_start_row = -1.0f;
   const float window_end_row   =  1.0f;
   const float window_start_col = -1.0f;
   const float window_end_col   =  1.0f;

   for (int ind = 0; ind < num_units; ind++)
   {
      if (floorf (z[ind]) == map_layer)
      {
         float x_pos = x[ind] / ((float)col_max) *
            (window_end_col - window_start_col) +
            window_start_col;

         float y_pos = y[ind] / ((float)row_max) *
            (window_end_row - window_start_row) +
            window_start_row;

         float point0[2] = { x_pos - 0.015f, y_pos - 0.015f };
         float point1[2] = { x_pos - 0.015f, y_pos + 0.015f };
         float point2[2] = { x_pos + 0.015f, y_pos + 0.015f };
         float point3[2] = { x_pos + 0.015f, y_pos - 0.015f };

         point0[0] += translation[0];
         point0[1] += translation[1];

         float temp;
         temp = point0[0];
         point0[0] = point0[0] * transform[0] + point0[1] * transform[1];// + translation[0];
         point0[1] = temp      * transform[2] + point0[1] * transform[3];// + translation[1];

         point1[0] += translation[0];
         point1[1] += translation[1];

         temp = point1[0];
         point1[0] = point1[0] * transform[0] + point1[1] * transform[1];// + translation[0];
         point1[1] = temp      * transform[2] + point1[1] * transform[3];// + translation[1];

         point2[0] += translation[0];
         point2[1] += translation[1];

         temp = point2[0];
         point2[0] = point2[0] * transform[0] + point2[1] * transform[1];// + translation[0];
         point2[1] = temp      * transform[2] + point2[1] * transform[3];// + translation[1];

         point3[0] += translation[0];
         point3[1] += translation[1];

         temp = point3[0];
         point3[0] = point3[0] * transform[0] + point3[1] * transform[1];// + translation[0];
         point3[1] = temp      * transform[2] + point3[1] * transform[3];// + translation[1];

         glBegin (GL_POLYGON);
         glColor3f (0.0f, 1.0f, 0.0f);
         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glEnd ();

         if (selected[ind])
         {
            float point0[2] = { x_pos - 0.018f, y_pos - 0.018f };
            float point1[2] = { x_pos - 0.018f, y_pos + 0.018f };
            float point2[2] = { x_pos + 0.018f, y_pos + 0.018f };
            float point3[2] = { x_pos + 0.018f, y_pos - 0.018f };

            point0[0] += translation[0];
            point0[1] += translation[1];

            float temp;
            temp = point0[0];
            point0[0] = point0[0] * transform[0] + point0[1] * transform[1];// + translation[0];
            point0[1] = temp      * transform[2] + point0[1] * transform[3];// + translation[1];

            point1[0] += translation[0];
            point1[1] += translation[1];

            temp = point1[0];
            point1[0] = point1[0] * transform[0] + point1[1] * transform[1];// + translation[0];
            point1[1] = temp      * transform[2] + point1[1] * transform[3];// + translation[1];

            point2[0] += translation[0];
            point2[1] += translation[1];

            temp = point2[0];
            point2[0] = point2[0] * transform[0] + point2[1] * transform[1];// + translation[0];
            point2[1] = temp      * transform[2] + point2[1] * transform[3];// + translation[1];

            point3[0] += translation[0];
            point3[1] += translation[1];

            temp = point3[0];
            point3[0] = point3[0] * transform[0] + point3[1] * transform[1];// + translation[0];
            point3[1] = temp      * transform[2] + point3[1] * transform[3];// + translation[1];

            glBegin (GL_LINES);
            glColor3f (1.0f, 0.0f, 0.0f);
            glVertex2f (point0[0], point0[1] );
            glVertex2f (point1[0], point1[1] );
            glVertex2f (point1[0], point1[1] );
            glVertex2f (point2[0], point2[1] );
            glVertex2f (point2[0], point2[1] );
            glVertex2f (point3[0], point3[1] );
            glVertex2f (point3[0], point3[1] );
            glVertex2f (point0[0], point0[1] );
            glEnd ();
         }
      }
   }
std::cout << std::endl;
}

void draw_selection_box (
      float *selection_box,
      float *transform,
      float *translation)
{
   float point0[2] = { selection_box[0], selection_box[1] };
   float point1[2] = { selection_box[2], selection_box[1] };
   float point2[2] = { selection_box[2], selection_box[3] };
   float point3[2] = { selection_box[0], selection_box[3] };

   point0[0] += translation[0];
   point0[1] += translation[1];

   float temp;
   temp = point0[0];
   point0[0] = point0[0] * transform[0] + point0[1] * transform[1];// + translation[0];
   point0[1] = temp      * transform[2] + point0[1] * transform[3];// + translation[1];

   point1[0] += translation[0];
   point1[1] += translation[1];

   temp = point1[0];
   point1[0] = point1[0] * transform[0] + point1[1] * transform[1];// + translation[0];
   point1[1] = temp      * transform[2] + point1[1] * transform[3];// + translation[1];

   point2[0] += translation[0];
   point2[1] += translation[1];

   temp = point2[0];
   point2[0] = point2[0] * transform[0] + point2[1] * transform[1];// + translation[0];
   point2[1] = temp      * transform[2] + point2[1] * transform[3];// + translation[1];

   point3[0] += translation[0];
   point3[1] += translation[1];

   temp = point3[0];
   point3[0] = point3[0] * transform[0] + point3[1] * transform[1];// + translation[0];
   point3[1] = temp      * transform[2] + point3[1] * transform[3];// + translation[1];

   glBegin (GL_LINES);
   glColor3f (1.0f, 0.0f, 0.0f);
   glVertex2f (point0[0], point0[1] );
   glVertex2f (point1[0], point1[1] );
   glVertex2f (point1[0], point1[1] );
   glVertex2f (point2[0], point2[1] );
   glVertex2f (point2[0], point2[1] );
   glVertex2f (point3[0], point3[1] );
   glVertex2f (point3[0], point3[1] );
   glVertex2f (point0[0], point0[1] );
   glEnd ();
}
