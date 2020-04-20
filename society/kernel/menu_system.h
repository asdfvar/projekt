#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "text.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

class Button
{
   public:

      Button (const std::string input_text, float ul_in[2], float width_in, float height_in)
      {
         text.populate (input_text);

         ul[0]  = ul_in[0];
         ul[1]  = ul_in[1];

         width  = width_in;
         height = height_in;
      }

      bool lclick (float x, float y)
      {
         if (ul[0] + width >= x && x >= ul[0] &&
               ul[1] >= y && y >= ul[0] - height)
            return true;

         return false;
      }

      bool lunclick (float x, float y)
      {
         if (ul[0] + width >= x && x >= ul[0] &&
               ul[1] >= y && y >= ul[0] - height)
            return true;

         return false;
      }

      void show (void)
      {

         text.display_contents (
               ul[0], // x
               ul[1], // y
               1.0f,  // alpha
               1.0f); // scale
         // draw the boundary of the button
         glColor3f (0.9f, 0.9f, 0.9f);

         glBegin (GL_POLYGON);
         glVertex2f (ul[0],         ul[1]          + 0.02f);
         glVertex2f (ul[0] + width, ul[1]          + 0.02f);
         glVertex2f (ul[0] + width, ul[1] - height + 0.02f);
         glVertex2f (ul[0],         ul[1] - height + 0.02f);
         glEnd ();
      }

  private:

      Text text;
      float ul[2];
      float width, height;
};

class BaseMenu
{
   public:

      virtual bool lclick (float x, float y)   { return false; };

      virtual bool lunclick (float x, float y) { return false; };

      virtual void show (void) { };

      virtual int get_menu_id (void) { return 0; };

   protected:

      float ul[2];
      float width, height;
      int menu_id;
};

class MainMenu : public BaseMenu
{
   public:

      MainMenu (void)
      {
         menu_id = 1;

         float ul[2] = { 0.0f, 0.0f };
         float button_width = 0.2f;
         float button_height = 0.1f;

         remove = new Button ("remove", ul, button_width, button_height);
      }

      ~MainMenu (void)
      {
         delete remove;
      }

      int get_menu_id (void) override { return menu_id; };

      bool lclick (float x, float y) override
      {
         if (remove->lclick (x, y))
         {
            // do stuff
            return true;
         }
         return false;
      }

      bool lunclick (float x, float y) override
      {
         if (remove->lunclick (x, y))
         {
            // do stuff
            return true;
         }
         return false;
      }


      void show (void) override
      {
         remove->show ();
      }

   private:

      Button *remove;
};

#endif
