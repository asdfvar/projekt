#include "text.h"
#include "menu_system.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

BuildMaterialMenu::BuildMaterialMenu (void) : BaseMenu ()
{
   menu_id = 3;

   width  = 0.3f;
   height = 0.3f;

   float button_width = 0.25f;
   float button_height = 0.1f;

   float button_ul[2];

   button_ul[0] =  0.02f;
   button_ul[1] = -0.09f;

   button_dirt  = new Button ("dirt",  button_ul, button_width, button_height);

   button_ul[0] =  0.02f;
   button_ul[1] = -0.2f;

   button_stone = new Button ("stone", button_ul, button_width, button_height);
}

BuildMaterialMenu::~BuildMaterialMenu (void)
{
   delete button_dirt;
   delete button_stone;
}

int BuildMaterialMenu::get_menu_id (void)
{
   return menu_id;
};

void BuildMaterialMenu::show (void)
{
   // Draw the remove-button menu
   button_dirt->show  (ul);
   button_stone->show (ul);

   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f (0.7f, 0.3f, 0.3f, alpha);

   // Draw the border
   glBegin (GL_POLYGON);
   glVertex3f (ul[0],         ul[1],          -0.2f);
   glVertex3f (ul[0] + width, ul[1],          -0.2f);
   glVertex3f (ul[0] + width, ul[1] - height, -0.2f);
   glVertex3f (ul[0],         ul[1] - height, -0.2f);
   glEnd ();

   menuBar.show (ul, width);
}

int BuildMaterialMenu::lclick (float x, float y)
{
   if (menuBar.hit (x, y, ul, width)) hit_menuBar = true;

   if (button_dirt->lclick  (x, y, ul)) return 1;
   if (button_stone->lclick (x, y, ul)) return 2;

   return 0;
}

int BuildMaterialMenu::lunclick (float x, float y)
{
   hit_menuBar = false;

   if (button_dirt->lunclick  (x, y, ul)) return 1;
   if (button_stone->lunclick (x, y, ul)) return 2;

   return 0;
}

void BuildMaterialMenu::translate (float dx, float dy)
{
   if (hit_menuBar)
   {
      move (dx, dy);
      menuBar.move (dx, dy);
   }
}
