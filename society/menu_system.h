#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "text.h"

class Button
{
   public:

      Button (const std::string input_text, float ul_in[2], float lr_in[2])
      {
         text.populate (input_text);

         ul[0] = ul_in[0];
         ul[1] = ul_in[1];

         lr[0] = lr_in[0];
         lr[1] = lr_in[1];
      }

      bool hit (float x, float y)
      {
         if (lr[0] >= x && x >= ul[0] &&
               ul[1] >= y && y >= lr[1])
            return true;

         return false;
      }

  private:

      Text text;
      float ul[2];
      float lr[2];
};

class BaseMenu
{
   public:

      virtual void click (float x, float y) { };

      virtual void show (void) { };
};

class MainMenu : public BaseMenu
{
   public:

      MainMenu (void)
      {
         float ul[2] = { 0.0f, 0.0f };
         float lr[2] = { 0.0f, 0.0f };

         build = new Button ("build", ul, lr);
      }

      ~MainMenu (void)
      {
         delete build;
      }

      void click (float x, float y) override
      {

      }

      void show (void) override
      {

      }

   private:

      Button *build;
};

#endif
