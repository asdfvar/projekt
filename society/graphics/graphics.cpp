#include "graphics.h"
#include "text.h"
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

void draw_actions (
      float       *transform,
      float       *translation,
      const int   *actions,
      int          num_actions,
      int          dims[3],
      int          map_layer)
{
   int row_max = dims[1];
   int col_max = dims[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float starting_row_loc = Y_START;
   const float starting_col_loc = X_START;

   // Draw the action cells
   for (int ind = 0; ind < num_actions; ind++)
   {
      if (actions[ind] / (dims[0] * dims[1]) != map_layer) continue;

      int cell = actions[ind] % (dims[0] * dims[1]);

      int col = cell % dims[0];
      int row = cell / dims[0];

      float vertex_x = starting_col_loc + block_width  * (float)col;
      float vertex_y = starting_row_loc + block_height * (float)row;

      glBegin (GL_POLYGON);
      glColor3f (1.0f, 0.0f, 0.0f);

      float point0[2] = { vertex_x,               vertex_y               };
      float point1[2] = { vertex_x + block_width, vertex_y               };
      float point2[2] = { vertex_x + block_width, vertex_y + block_width };
      float point3[2] = { vertex_x,               vertex_y + block_width };

      transformation (point0, transform, translation);
      transformation (point1, transform, translation);
      transformation (point2, transform, translation);
      transformation (point3, transform, translation);

      glVertex2f (point0[0], point0[1]);
      glVertex2f (point1[0], point1[1]);
      glVertex2f (point2[0], point2[1]);
      glVertex2f (point3[0], point3[1]);
      glEnd ();
   }
}

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

   // Draw grid lines
   glLineWidth (0.1f);
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

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glBegin (GL_LINES);
         glColor3f (0.0f, 0.0f, 1.0f);
         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glVertex2f (point3[0], point3[1]);
         glVertex2f (point0[0], point0[1]);
         glEnd();
      }
   }

   // Draw cells
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

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
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

   const float width_offset  = 0.3f * block_width;
   const float height_offset = 0.3f * block_height;

   const float selected_width_offset  = 0.35f * block_width;
   const float selected_height_offset = 0.35f * block_height;

   glLineWidth (0.1f);
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

         if (selected[ind])
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

void draw_selection_box (
      float selection_box[2][3],
      float *transform,
      float *translation)
{
   float point0[2] = { selection_box[0][0], selection_box[0][1] };
   float point1[2] = { selection_box[1][0], selection_box[0][1] };
   float point2[2] = { selection_box[1][0], selection_box[1][1] };
   float point3[2] = { selection_box[0][0], selection_box[1][1] };

   transformation (point0, transform, translation);
   transformation (point1, transform, translation);
   transformation (point2, transform, translation);
   transformation (point3, transform, translation);

   glLineWidth (0.1f);
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

void HUD::draw_info (void)
{
   float first_time_component = 0.25f * display_time_limit;

   float alpha_start = 0.8f;
   float alpha = alpha_start;

   if (accumulated_time > first_time_component)
   {
      alpha = 1.0f -
         (accumulated_time   - first_time_component) /
         (display_time_limit - first_time_component);

      alpha *= alpha_start;
   }

   // Display text with a fading decay
   if (accumulated_time <= display_time_limit)
   {
      Text elevation;

      elevation.populate ("elevation: ");
      elevation.populate (map_layer);

      elevation.display_contents (-0.95f, 0.95f, alpha, 1.0f);
   }

   if (mode == 1)
   {
      Text dig;

      dig.populate ("dig mode");
      dig.display_contents (0.70f, 0.95f, 0.6f, 1.0f);
   }
}

void HUD::update (float time_step, int map_layer_in)
{
   if (map_layer_in != map_layer)
   {
      accumulated_time = 0.0f;
      map_layer = map_layer_in;
   }

   accumulated_time += time_step;
}
