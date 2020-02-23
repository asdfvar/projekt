#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "text.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

class Graphics
{
   public:

      Graphics (void) { };

      void draw_selection_box (
            float selection_box[2][3],
            float *transform,
            float *translation);
};

class HUD
{
   public:

      HUD (void)
      {
         accumulated_time   = 0.0f;
         display_time_limit = 2.0f;
         map_layer          = 0;
         mode               = 0;
      }

      void draw_info (void);
      void update (float time_step, int map_layer_in);

      void set_mode (int mode_in) { mode = mode_in; };
      void unset_mode (void)      { mode = 0;       };

   private:

      float accumulated_time;
      float display_time_limit;
      int   map_layer;
      int   mode;
};

#endif
