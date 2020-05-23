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
         alpha = 0.8f;

         ul[0] = -0.8f;
         ul[1] =  0.8f;
      }

      void reset (void)
      {
         ul[0] = -0.8f;
         ul[1] =  0.8f;
      }

      void move (float dx, float dy)
      {
         ul[0] += dx;
         ul[1] += dy;
      }

      virtual void show        (void) { };

   protected:

      float alpha;
      float ul[2];
      float width;
      float height;
};

class Button : public MenuSystem
{
   public:

      Button (const std::string input_text, float ul_in[2], float width_in, float height_in);
      bool lclick   (float x, float y, float ul[2]);
      bool lunclick (float x, float y, float ul[2]);
      void show     (float ul[2]);

  private:

      Text  text;
      float width, height;
};

class MenuBar : public MenuSystem
{
   public:

      MenuBar (void);
      void show (float ul[2], float width);
      bool hit  (float x, float y, float ul[2], float width);
};

class BaseMenu : public MenuSystem
{
   public:

      BaseMenu (void) : MenuSystem () { hit_menuBar = false; };

      virtual int  lclick      (float x, float y) { return false; };
      virtual int  lunclick    (float x, float y) { return false; };
      virtual int  get_menu_id (void) { return 0; };
      virtual void translate   (float dx, float dy) { };

   protected:

      float   width, height;
      int     menu_id;
      MenuBar menuBar;
      bool    hit_menuBar;
};

class MainMenu : public BaseMenu
{
   public:

      MainMenu (void);
      ~MainMenu (void);

      int  get_menu_id (void)               override;
      int  lclick      (float x,  float y ) override;
      int  lunclick    (float x,  float y ) override;
      void translate   (float dx, float dy) override;
      void show        (void)               override;

   private:

      Button *button_remove;
      Button *button_build;
};

class BuildMenu : public BaseMenu
{
   public:

      BuildMenu (void);
      ~BuildMenu (void);

      int  get_menu_id (void)               override;
      int  lclick      (float x,  float y ) override;
      int  lunclick    (float x,  float y ) override;
      void translate   (float dx, float dy) override;
      void show        (void)               override;

   private:

      Button *button_material;
      Button *button_item;
};

class BuildMaterialMenu : public BaseMenu
{
   public:

      BuildMaterialMenu (void);
      ~BuildMaterialMenu (void);

      int  get_menu_id (void)               override;
      int  lclick      (float x,  float y ) override;
      int  lunclick    (float x,  float y ) override;
      void translate   (float dx, float dy) override;
      void show        (void)               override;

   private:

      Button *button_dirt;
      Button *button_stone;
};

#endif
