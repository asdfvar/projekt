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

bool Button::lclick (float x, float y)
{
   if (ul[0] + width >= x && x >= ul[0] &&
         ul[1] >= y && y >= ul[1] - height)
      return true;

   return false;
}

bool Button::lunclick (float x, float y)
{
   if (ul[0] + width >= x && x >= ul[0] &&
         ul[1] >= y && y >= ul[1] - height)
      return true;

   return false;
}

void Button::show (void)
{
   text.display_contents (
         ul[0], // x
         ul[1], // y
         1.0f,  // alpha
         1.0f); // scale

   // draw the boundary of the button
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f (0.9f, 0.9f, 0.9f, alpha);

   glBegin (GL_POLYGON);
   glVertex3f (ul[0],         ul[1]          + 0.02f, -0.5f);
   glVertex3f (ul[0] + width, ul[1]          + 0.02f, -0.5f);
   glVertex3f (ul[0] + width, ul[1] - height + 0.02f, -0.5f);
   glVertex3f (ul[0],         ul[1] - height + 0.02f, -0.5f);
   glEnd ();
}

MainMenu::MainMenu (void) : BaseMenu ()
{
   menu_id = 1;

   width  = 0.5f;
   height = 0.5f;

   ul[0] = -0.8f;
   ul[1] =  0.8f;

   float button_width = 0.2f;
   float button_height = 0.1f;

   float button_ul[2];

   button_ul[0] = ul[0] + 0.1f;
   button_ul[1] = ul[1] - 0.1f;

   button_remove = new Button ("remove", button_ul, button_width, button_height);

   button_ul[0] = ul[0] + 0.1f;
   button_ul[1] = ul[1] - 0.4f;

   button_build  = new Button ("build", button_ul, button_width, button_height);
}

MainMenu::~MainMenu (void)
{
   delete button_remove;
   delete button_build;
}

int MainMenu::get_menu_id (void)
{
   return menu_id;
};

int MainMenu::lclick (float x, float y)
{
   if (button_remove->lclick (x, y))
      return 1;

   if (button_build->lclick (x, y))
      return 2;

   return 0;
}

int MainMenu::lunclick (float x, float y)
{
   if (button_remove->lunclick (x, y))
      return 1;

   if (button_build->lunclick (x, y))
      return 2;

   return 0;
}


void MainMenu::show (void)
{
   // Draw the remove-button menu
   button_remove->show ();
   button_build->show ();

   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f (0.7f, 0.2f, 0.3f, alpha);

   // Draw the border
   glBegin (GL_POLYGON);
   glVertex3f (ul[0],         ul[1],          -0.5f);
   glVertex3f (ul[0] + width, ul[1],          -0.5f);
   glVertex3f (ul[0] + width, ul[1] - height, -0.5f);
   glVertex3f (ul[0],         ul[1] - height, -0.5f);
   glEnd ();
}
