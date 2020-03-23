#include "x01.h"
#include "utils.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cmath>

X01::X01 (float position_x, float position_y, float position_z, MAP* Map) : Unit { position_x, position_y, position_z, Map } { }
X02::X02 (float position_x, float position_y, float position_z, MAP* Map) : Unit { position_x, position_y, position_z, Map } { }

void X01::draw (float *transform, float *translation)
{
   int map_layer = Map->get_map_layer ();

   float col_max = static_cast<float> (Map->shape (0));
   float row_max = static_cast<float> (Map->shape (1));

   // Block sizes in the viewing window
   float block_width  = 2.0f / col_max;
   float block_height = 2.0f / row_max;

   if (floorf (position[2]) <= map_layer)
   {
      int map_2d_ind[2] = { (int)position[0], (int)position[1] };

      if (position[2] >= Map->get_view_plain (map_2d_ind))
      {
         // Transform map position to view-window position
         float x_pos = map_to_window (position[0], col_max);

         // Transform map position to view-window position
         float y_pos = map_to_window (position[1], (float)row_max);

         float reduction_factor = 1.0f - (float)(map_layer - position[2]) / 8.0f;
         if (reduction_factor < 0.05f) reduction_factor = 0.05f;

         float arg = static_cast<float> (tic % 100) * 0.02f * 3.14159f * 2.0f;
         float sin_tic = sinf (arg);
         float width_offset  = (0.25f + 0.05f * sin_tic) * block_width  * reduction_factor;
         float height_offset = (0.25f + 0.05f * sin_tic) * block_height * reduction_factor;

         float point0[2] = { x_pos - width_offset, y_pos - height_offset };
         float point1[2] = { x_pos - width_offset, y_pos + height_offset };
         float point2[2] = { x_pos + width_offset, y_pos + height_offset };
         float point3[2] = { x_pos + width_offset, y_pos - height_offset };

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glBegin (GL_POLYGON);
         if (!Map->test_los (position))
            glColor3f (0.3f, 0.3f, 0.0f);
         else
            glColor3f (1.0f, 1.0f, 0.0f);

         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glEnd ();

         if (selected)
         {
            glLineWidth (0.1f);

            const float selected_width_offset  = 0.35f * block_width;
            const float selected_height_offset = 0.35f * block_height;

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

   tic++;
}

void X02::draw (float *transform, float *translation)
{
   int map_layer = Map->get_map_layer ();

   float col_max = static_cast<float> (Map->shape (0));
   float row_max = static_cast<float> (Map->shape (1));

   float block_width  = 2.0f / col_max;
   float block_height = 2.0f / row_max;

   if (floorf (position[2]) <= map_layer)
   {
      int map_2d_ind[2] = { (int)position[0], (int)position[1] };

      if (position[2] >= Map->get_view_plain (map_2d_ind))
      {
         // Transform map position to view-window position
         float x_pos = map_to_window (position[0], col_max);

         // Transform map position to view-window position
         float y_pos = map_to_window (position[1], (float)row_max);

         float reduction_factor = 1.0f - (float)(map_layer - position[2]) / 8.0f;
         if (reduction_factor < 0.05f) reduction_factor = 0.05f;

         float width_offset  = 0.3f * block_width  * reduction_factor;
         float height_offset = 0.3f * block_height * reduction_factor;

         float point0[2] = { x_pos - width_offset, y_pos - height_offset };
         float point1[2] = { x_pos - width_offset, y_pos + height_offset };
         float point2[2] = { x_pos + width_offset, y_pos + height_offset };
         float point3[2] = { x_pos + width_offset, y_pos - height_offset };

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glBegin (GL_POLYGON);
         if (!Map->test_los (position))
            glColor3f (0.0f, 0.3f, 0.12f);
         else
            glColor3f (0.0f, 1.0f, 0.4f);

         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glEnd ();

         if (selected)
         {
            glLineWidth (0.1f);

            const float selected_width_offset  = 0.35f * block_width;
            const float selected_height_offset = 0.35f * block_height;

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
