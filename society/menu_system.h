#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "text.h"

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

      bool hit (float x, float y)
      {
         if (ul[0] + width >= x && x >= ul[0] &&
               ul[1] >= y && y >= ul[0] - height)
            return true;

         return false;
      }

      void show (void)
      {
         text.display_contents (ul[0], ul[1], 1.0f, 1.0f);
      }

  private:

      Text text;
      float ul[2];
      float width, height;
};

class BaseMenu
{
   public:

      virtual void click (float x, float y) { };

      virtual void show (void) { };

   protected:


};

class MainMenu : public BaseMenu
{
   public:

      MainMenu (void)
      {
         float ul[2] = { 0.0f, 0.0f };
         float width = 0.2f;
         float height = 0.1f;

         build = new Button ("build", ul, width, height);
      }

      ~MainMenu (void)
      {
         delete build;
      }

      void click (float x, float y) override
      {
         if (build->hit (x, y))
         {
            // do stuff
         }
      }

      void show (void) override
      {
         build->show ();
      }

   private:

      Button *build;
};

#endif
