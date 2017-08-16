#include "opengl_interface.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <iostream>

static float max_view_distance = 100.0f;

/* function name: opengl_settings */
void ogl::opengl_initial_settings( void )
{

   float light_ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
   float light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
   float light_specular[] = { 0.8, 0.8, 0.8, 1.0 };
   float light_position[] = { 0.0, 0.0, 1.0, 0.0 };
   
   glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient   );
   glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse   );
   glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );
   glLightfv( GL_LIGHT0, GL_POSITION, light_position );
   
   glEnable( GL_LIGHT0         );
   glEnable( GL_LESS           );
   glEnable( GL_DEPTH_TEST     );
   glEnable( GL_LIGHTING       );
   glEnable( GL_COLOR_MATERIAL );

   glDepthFunc( GL_LESS );

}

/* function name: opengl_clear */
void ogl::opengl_clear( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/* function name: swap_buffers */
void ogl::swap_buffers( void )
{
   glutSwapBuffers();
}

/* function name: redisplay */
void ogl::redisplay ( void )
{
   glutPostRedisplay();
}

void ogl::warp_pointer( int x, int y )
{
   glutWarpPointer( x, y );
}

void ogl::set_cursor_none( void )
{
   glutSetCursor( GLUT_CURSOR_NONE );
}

void ogl::set_cursor_arrow( void )
{
   glutSetCursor( GLUT_CURSOR_LEFT_ARROW );
}

/* function name: draw_polygon */
void ogl::draw_polygon( float *vertices_x,
                        float *vertices_y,
                        float *distances,
                        float *color,
                        float  brightness,
                        int    num_vertices )
{

   glBegin(GL_POLYGON);
     glNormal3f(0.0f, 0.0f, -brightness);
     glColor3f( color[0], color[1], color[2]);
     for (int vertex = 0; vertex < num_vertices; vertex++)
     {
        glVertex3f(vertices_x[vertex], vertices_y[vertex], distances[vertex] / max_view_distance);
     }
   glEnd();

}

void ogl::draw_2d_polygon( float *vertices_x,
                           float *vertices_y,
                           float *color,
                           int    num_vertices )
{

   glBegin(GL_POLYGON);

     glColor3f( color[0], color[1], color[2] );
     for (int vertex = 0; vertex < num_vertices; vertex++)
     {
        glVertex2f( vertices_x[vertex], vertices_y[vertex] );
     }

   glEnd();

}

void ogl::vertex( float x, float y, float z )
{
   glVertex3f( x, y, z );
}

void ogl::color( float x, float y, float z )
{
   glColor3f( x, y, z );
}

void ogl::line_width( float width )
{
   glLineWidth( width );
}

void ogl::begin_lines( void )
{
   glBegin( GL_LINES );
}

int ogl::get_window_width( void )
{
   return glutGet( GLUT_WINDOW_WIDTH );
}

int ogl::get_window_height( void )
{
   return glutGet( GLUT_WINDOW_HEIGHT );
}

void ogl::normal( float x, float y, float z )
{
   glNormal3f( x, y, z );
}

void ogl::end( void )
{
   glEnd();
}

bool ogl::key_f1( int key )
{
   return key == GLUT_KEY_F1;
}
