#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>

class Button
{
   public:
      Button( float position[2],
              float width,
              float height);

      void click( float position[2]);
      void show( void );

   private:
      float position[2];
      float width;
      float height;
      std::string text;
};

class Menu
{
   public:
      Menu( void );

};

#endif
