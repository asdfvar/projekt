#ifndef OPENGL_INTERFACE_H
#define OPENGL_INTERFACE_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

namespace ogl
{

   void opengl_initial_settings ( void );

   void opengl_clear ( void );

   void swap_buffers ( void );

   void redisplay ( void );

   void draw_polygon( float *vertices_x,
                      float *vertices_y,
                      float *distances,
                      float *color,
                      float  brightness,
                      int    num_vertices);

}

#endif
