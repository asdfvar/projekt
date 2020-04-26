#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "text.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

class MenuSystem
{
   public:

      MenuSystem (void)
      {
         alpha = 0.7f;
      }

   protected:

      float alpha;
};

class Button : public MenuSystem
{
   public:

      Button (const std::string input_text, float ul_in[2], float width_in, float height_in);

      bool lclick (float x, float y);

      bool lunclick (float x, float y);

      void show (void);

  private:

      Text  text;
      float ul[2];
      float width, height;
};

class BaseMenu : public MenuSystem
{
   public:

      BaseMenu (void) : MenuSystem () { };

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

      MainMenu (void);

      ~MainMenu (void);

      int get_menu_id (void) override;

      bool lclick (float x, float y) override;

      bool lunclick (float x, float y) override;

      void show (void) override;

   private:

      Button *button_remove;
};

#endif
