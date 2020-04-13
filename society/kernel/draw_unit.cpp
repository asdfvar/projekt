#include "derivative_units.h"
#include "utils.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cmath>

X01::X01 (float position[3], MAP* Map) : Unit { position, Map } { }
X02::X02 (float position[3], MAP* Map) : Unit { position, Map } { }

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
         float x_pos = map_to_window (position[0], (float)col_max);
         float y_pos = map_to_window (position[1], (float)row_max);

         float reduction_factor = 1.0f - (float)(map_layer - position[2]) / 8.0f;
         if (reduction_factor < 0.05f) reduction_factor = 0.05f;

         float arg = static_cast<float> (tic % 50) * 0.02f * 3.14159f * 2.0f;
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
         float x_pos = map_to_window (position[0], (float)col_max);
         float y_pos = map_to_window (position[1], (float)row_max);

         float reduction_factor = 1.0f - (float)(map_layer - position[2]) / 8.0f;
         if (reduction_factor < 0.05f) reduction_factor = 0.05f;

         float width_offset  = 0.3f * block_width  * reduction_factor;
         float height_offset = 0.3f * block_height * reduction_factor;

         float arg = (static_cast<float> (tic % 50) * 0.02f) * 3.14159f * 2.0f;
         arg = cosf (arg);       // map to [-1, 1]
         arg *= 3.14159f * .25f; // map to [-1, 1] * 1 / 4 * pi
         float sin_tic = sinf (arg);
         float cos_tic = cosf (arg);

         float rot[4] = {
            cos_tic, -sin_tic,
            sin_tic,  cos_tic };

         float point_xoffset[4];
         float point_yoffset[4];

         point_xoffset[0] = -0.3f * block_width * reduction_factor;
         point_xoffset[1] = -0.3f * block_width * reduction_factor;
         point_xoffset[2] =  0.3f * block_width * reduction_factor;
         point_xoffset[3] =  0.3f * block_width * reduction_factor;

         point_yoffset[0] = -0.3f * block_height * reduction_factor;
         point_yoffset[1] =  0.3f * block_height * reduction_factor;
         point_yoffset[2] =  0.3f * block_height * reduction_factor;
         point_yoffset[3] = -0.3f * block_height * reduction_factor;

         float x, y;

         x = point_xoffset[0] * rot[0] + point_yoffset[0] * rot[1];
         y = point_xoffset[0] * rot[2] + point_yoffset[0] * rot[3];
         point_xoffset[0] = x;
         point_yoffset[0] = y;

         x = point_xoffset[1] * rot[0] + point_yoffset[1] * rot[1];
         y = point_xoffset[1] * rot[2] + point_yoffset[1] * rot[3];
         point_xoffset[1] = x;
         point_yoffset[1] = y;

         x = point_xoffset[2] * rot[0] + point_yoffset[2] * rot[1];
         y = point_xoffset[2] * rot[2] + point_yoffset[2] * rot[3];
         point_xoffset[2] = x;
         point_yoffset[2] = y;

         x = point_xoffset[3] * rot[0] + point_yoffset[3] * rot[1];
         y = point_xoffset[3] * rot[2] + point_yoffset[3] * rot[3];
         point_xoffset[3] = x;
         point_yoffset[3] = y;

         float point0[2] = { x_pos + point_xoffset[0], y_pos + point_yoffset[0] };
         float point1[2] = { x_pos + point_xoffset[1], y_pos + point_yoffset[1] };
         float point2[2] = { x_pos + point_xoffset[2], y_pos + point_yoffset[2] };
         float point3[2] = { x_pos + point_xoffset[3], y_pos + point_yoffset[3] };

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
