#include "hud.h"

/*
** OpenGL includes
*/
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

namespace hud
{

  void display( void)
  {

     /*
     ** Draw reticle
     */
     const float width_height = 0.05f;
     float       half_width_height = 0.5f * width_height;

     glNormal3f(0.0f, 0.0f, 1.0f);

     /*
     ** Black interior lines
     */
     glColor3f( 0.0f, 0.0f, 0.0f);
     glLineWidth(2.5f);
     glBegin(GL_LINES);
       glVertex3f(-half_width_height + 0.005f, 0.0f, 0.0f);
       glVertex3f(half_width_height - 0.005f, 0.0f, 0.0f);
       glBegin(GL_LINES);
       glVertex3f(0.0f, -half_width_height + 0.005f, 0.0f);
       glVertex3f(0.0f, half_width_height - 0.005f, 0.0f);
     glEnd();

     /*
     ** White-thicker exterior lines
     */
     glColor3f( 1.0f, 1.0f, 1.0f);
     glLineWidth(3.5f);
     glBegin(GL_LINES);
       glVertex3f(-half_width_height, 0.0f, 0.0f);
       glVertex3f(half_width_height, 0.0f, 0.0f);
       glBegin(GL_LINES);
       glVertex3f(0.0f, -half_width_height, 0.0f);
       glVertex3f(0.0f, half_width_height, 0.0f);
     glEnd();


  }

}
