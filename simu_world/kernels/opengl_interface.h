#ifndef OPENGL_INTERFACE_H
#define OPENGL_INTERFACE_H

inline
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

   void draw_2d_polygon( float *vertices_x,
                         float *vertices_y,
                         float *color,
                         int    num_vertices);

   void vertex( float x, float y, float z );
   void color( float x, float y, float z );
   void line_width( float );
   void begin_lines( void );
   void end( void );

}

#endif
