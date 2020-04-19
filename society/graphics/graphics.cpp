#include "graphics.h"
#include "text.h"
#include "mode.h"
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

void Graphics::draw_selection_box (
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

   if (mode == mode::REMOVE)
   {
      Text remove;

      remove.populate ("remove mode");
      remove.display_contents (0.60f, 0.95f, 0.7f, 1.0f);
   }

   if (mode == mode::BUILD)
   {
      Text build;

      build.populate ("build mode");
      build.display_contents (0.60f, 0.95f, 0.7f, 1.0f);
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
