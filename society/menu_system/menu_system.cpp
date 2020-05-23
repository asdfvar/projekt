#include "text.h"
#include "menu_system.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

Button::Button (const std::string input_text, float ul_in[2], float width_in, float height_in) : MenuSystem ()
{
   text.populate (input_text);

   ul[0]  = ul_in[0];
   ul[1]  = ul_in[1];

   width  = width_in;
   height = height_in;
}

bool Button::lclick (float x, float y, float menu_ul[2])
{
   if (menu_ul[0] + ul[0] + width >= x && x >= menu_ul[0] + ul[0] &&
         menu_ul[1] + ul[1] >= y && y >= menu_ul[1] + ul[1] - height)
      return true;

   return false;
}

bool Button::lunclick (float x, float y, float menu_ul[2])
{
   if (menu_ul[0] + ul[0] + width >= x && x >= menu_ul[0] + ul[0] &&
         menu_ul[1] + ul[1] >= y && y >= menu_ul[1] + ul[1] - height)
      return true;

   return false;
}

void Button::show (float menu_ul[2])
{
   text.display_contents (
         menu_ul[0] + ul[0], // x
         menu_ul[1] + ul[1], // y
         alpha, // alpha
         1.0f); // scale

   // draw the boundary of the button
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f (0.9f, 0.9f, 0.9f, alpha);

   glBegin (GL_POLYGON);
   glVertex3f (menu_ul[0] + ul[0],         menu_ul[1] + ul[1]          + 0.02f, -0.3f);
   glVertex3f (menu_ul[0] + ul[0] + width, menu_ul[1] + ul[1]          + 0.02f, -0.3f);
   glVertex3f (menu_ul[0] + ul[0] + width, menu_ul[1] + ul[1] - height + 0.02f, -0.3f);
   glVertex3f (menu_ul[0] + ul[0],         menu_ul[1] + ul[1] - height + 0.02f, -0.3f);
   glEnd ();
}

MenuBar::MenuBar (void) : MenuSystem () { }

void MenuBar::show (float ul[2], float width)
{
   // draw the boundary of the button
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f (0.1f, 0.1f, 0.1f, alpha);

   glBegin (GL_POLYGON);
   glVertex3f (ul[0]         + 0.01f, ul[1] - 0.01f, -0.3f);
   glVertex3f (ul[0] + width - 0.01f, ul[1] - 0.01f, -0.3f);
   glVertex3f (ul[0] + width - 0.01f, ul[1] - 0.04f, -0.3f);
   glVertex3f (ul[0]         + 0.01f, ul[1] - 0.04f, -0.3f);
   glEnd ();
}

bool MenuBar::hit (float x, float y, float ul[2], float width)
{
   if (ul[0] <= x && x <= ul[0] + width &&
         ul[1] - 0.04f <= y && y <= ul[1]) return true;

   return false;
}
