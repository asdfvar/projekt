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
      float selection_box[2][3],
      float *transform,
      float *translation);

void draw_actions (
      float       *transform,
      float       *translation,
      const int   *actions,
      int          num_actions,
      int          dims[3],
      int          map_layer);

class HUD
{
   public:

      HUD (void)
      {
         accumulated_time   = 0.0f;
         display_time_limit = 2.0f;
         map_layer          = 0;
      }

      void draw_info (void);
      void update (float time_step, int map_layer_in);

   private:

      float accumulated_time;
      float display_time_limit;
      int   map_layer;
};

#endif
