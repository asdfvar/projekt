#include "opengl_interface.h"
#include <iostream>

static float max_view_distance = 100.0f;

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
