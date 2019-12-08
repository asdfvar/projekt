#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>

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

#endif
