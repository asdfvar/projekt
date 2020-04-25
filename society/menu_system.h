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

         alpha = 0.7f;
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
         glEnable (GL_BLEND);
         glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glColor4f (0.9f, 0.9f, 0.9f, alpha);

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
      float alpha;
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
      float alpha;
};

class MainMenu : public BaseMenu
{
   public:

      MainMenu (void)
      {
         menu_id = 1;
         alpha = 0.7f;

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
      }

      ~MainMenu (void)
      {
         delete button_remove;
      }

      int get_menu_id (void) override { return menu_id; };

      bool lclick (float x, float y) override
      {
         if (button_remove->lclick (x, y))
         {
            // do stuff
            return true;
         }
         return false;
      }

      bool lunclick (float x, float y) override
      {
         if (button_remove->lunclick (x, y))
         {
            // do stuff
            return true;
         }
         return false;
      }


      void show (void) override
      {
         // Draw the remove-button menu
         button_remove->show ();

         glEnable (GL_BLEND);
         glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glColor4f (0.7f, 0.2f, 0.3f, alpha);

         // Draw the border
         glBegin (GL_POLYGON);
         glVertex2f (ul[0],         ul[1]);
         glVertex2f (ul[0] + width, ul[1]);
         glVertex2f (ul[0] + width, ul[1] - height);
         glVertex2f (ul[0],         ul[1] - height);
         glEnd ();
      }

   private:

      Button *button_remove;
};

#endif
