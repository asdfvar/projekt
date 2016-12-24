#include "opengl_interface.h"
#include <iostream>

static float max_view_distance = 100.0f;

/* function name: opengl_settings */
void ogl::opengl_initial_settings( void )
{

   float light_ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
   float light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
   float light_specular[] = { 0.8, 0.8, 0.8, 1.0 };
   float light_position[] = { 0.0, 0.0, 1.0, 0.0 };
   
   glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv( GL_LIGHT0, GL_POSITION, light_position);
   
   glEnable( GL_LIGHT0         );
   glEnable( GL_LESS           );
   glEnable( GL_DEPTH_TEST     );
   glEnable( GL_LIGHTING       );
   glEnable( GL_COLOR_MATERIAL );

}

/* function name: opengl_clear */
void ogl::opengl_clear( void )
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* function name: draw_polygon */
void ogl::draw_polygon( float *vertices_x,
                        float *vertices_y,
                        float *distances,
                        float *color,
                        float  brightness,
                        int    num_vertices)
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
