#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "text.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

void draw_map (
      float       *tranform,
      float       *translation,
      const float *map,
      int          map_dims[3],
      int          map_layer);

void draw_units (
      float *tranform,
      float *translation,
      float *x,
      float *y,
      float *z,
      bool  *selected,
      int    map_dims[3],
      int    num_units,
      int    map_layer);

void draw_selection_box (
      float *selection_box,
      float *transform,
      float *translation);

class HUD
{
   public:

      HUD (void)
      {
         accumulated_time   = 0.0f;
         display_time_limit = 1.0f;
         map_layer          = 0;
      }

      void draw_info (void)
      {
         Text text;

         text.populate (map_layer);

         float first_time_component = 0.25f * display_time_limit;

         float alpha = 1.0f;
         if (accumulated_time > first_time_component)
         {
            alpha = 1.0f -
               (accumulated_time   - first_time_component) /
               (display_time_limit - first_time_component);
         }

         if (accumulated_time <= display_time_limit)
         {
            text.display_contents (-0.95f, 0.95f, alpha, 1.0f);
         }
      }

      void update (float time_step, int map_layer_in)
      {
         if (map_layer_in != map_layer)
         {
            accumulated_time = 0.0f;
            map_layer = map_layer_in;
         }

         accumulated_time += time_step;
      }

   private:

      float accumulated_time;
      float display_time_limit;
      int   map_layer;
};

#endif
