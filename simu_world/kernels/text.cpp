#include "text.h"
#include <iostream>
#include <stdio.h>

/*
** OpenGL includes
*/
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

namespace txt
{
   void write_to_screen( std::string input,
                         float x,
                         float y)
   {

      for (int k = 0; k < input.length(); k++)
      {
         switch( input.at(k) )
         {
            case 'A':
               glColor3f( 0.0f, 1.0f, 0.0f);
               glLineWidth(3.0f);
               glBegin(GL_LINES);
                 glVertex3f(x - 0.01f, y - 0.02f, 0.0f);
                 glVertex3f(x - 0.01f, y,         0.0f);
                 glVertex3f(x - 0.01f, y,         0.0f);
                 glVertex3f(x,         y + 0.02f, 0.0f);
                 glVertex3f(x,         y + 0.02f, 0.0f);
                 glVertex3f(x + 0.01f, y,         0.0f);
                 glVertex3f(x + 0.01f, y,         0.0f);
                 glVertex3f(x + 0.01f, y - 0.02f, 0.0f);
                 glVertex3f(x - 0.01f, y,         0.0f);
                 glVertex3f(x + 0.01f, y,         0.0f);
               glEnd();
               break;
         }
      }
   }
}
