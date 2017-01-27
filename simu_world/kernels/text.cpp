#include "text.h"
#include <iostream>
#include <stdio.h>

/*
** OpenGL includes
*/
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

/*             1         2
**        **********************
**        **********************
**        ***       **       ***
**        ** *      **      * **
**        **  *   5 **     *  **
**        **   *    **  6 *   **
**      3 **  4 *   **   *   7**
**        **     *  **  *     **
**        **      * ** *      **
**        **  8    ****   9   **
**        **********************
**        **********************
**        **       ****       **
**        **      * ** *      **
**        **  11 *  **  *  14 **
**     10 **    *   **   *    **
**        **   *    **    *   **
**        **  *  12 **  13 *  **
**        ** *      **      * **
**        ***       **       ***
**        **********************
**        **********************
**             15       16
*/
static void LCD_font(unsigned int plick,
                     const float  x,
                     const float  y,
                     const float  scale)
{
   const float hor = 0.01f;
   const float ver = 0.02f;

   switch( plick )
   {
      case 1:
         glVertex3f(x - hor * scale, y + ver * scale, 0.0f);
         glVertex3f(x,               y + ver * scale, 0.0f);
        break;

      case 2:
         glVertex3f(x,               y + ver * scale, 0.0f);
         glVertex3f(x + hor * scale, y + ver * scale, 0.0f);
        break;

      case 3:
         glVertex3f(x - hor * scale, y,               0.0f);
         glVertex3f(x - hor * scale, y + ver * scale, 0.0f);
        break;

      case 4:
         glVertex3f(x - hor * scale, y + ver * scale, 0.0f);
         glVertex3f(x,               y,               0.0f);
        break;

      case 5:
         glVertex3f(x,               y + ver * scale, 0.0f);
         glVertex3f(x,               y,               0.0f);
        break;

      case 6:
         glVertex3f(x,               y,               0.0f);
         glVertex3f(x + hor * scale, y + ver * scale, 0.0f);
        break;

      case 7:
         glVertex3f(x + hor * scale, y,               0.0f);
         glVertex3f(x + hor * scale, y + ver * scale, 0.0f);
        break;

      case 8:
         glVertex3f(x - hor * scale, y,               0.0f);
         glVertex3f(x,               y,               0.0f);
        break;

      case 9:
         glVertex3f(x,               y,               0.0f);
         glVertex3f(x + hor * scale, y,               0.0f);
        break;
   }
}

/*
** function name: write_to_screen from: Text
*/
void Text::write_to_screen( std::string input,
                            const float x,
                            const float y,
                            const float scale)
{

   const float hor = 0.01f;
   const float ver = 0.02f;

   float offset = 0.0f;

   for (int k = 0; k < input.length(); k++)
   {

      // setup the GL conditions
      glColor3f( 0.0f, 1.0f, 0.0f );
      glLineWidth( 3.0f );
      glBegin( GL_LINES );

      switch( input.at(k) )
      {
         case 'A':
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y,               0.0f);
            glVertex3f(offset + x - hor * scale, y,               0.0f);
            glVertex3f(offset + x,               y + ver * scale, 0.0f);
            glVertex3f(offset + x,               y + ver * scale, 0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            break;

         case 'B':
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y + ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y + ver * scale, 0.0f);
            glVertex3f(offset + x,               y + ver * scale, 0.0f);
            glVertex3f(offset + x,               y + ver * scale, 0.0f);
            glVertex3f(offset + x,               y,               0.0f);
            glVertex3f(offset + x,               y,               0.0f);
            glVertex3f(offset + x - hor * scale, y,               0.0f);
            glVertex3f(offset + x - hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x + hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            break;

         case 'C':
            glVertex3f(offset + x + hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y + ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y + ver * scale, 0.0f);
            glVertex3f(offset + x + hor * scale, y + ver * scale, 0.0f);
            break;

         case 'D':
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y + ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y + ver * scale, 0.0f);
            glVertex3f(offset + x,               y + ver * scale, 0.0f);
            glVertex3f(offset + x,               y + ver * scale, 0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            glVertex3f(offset + x + hor * scale, y,               0.0f);
            glVertex3f(offset + x,               y - ver * scale, 0.0f);
            glVertex3f(offset + x,               y - ver * scale, 0.0f);
            glVertex3f(offset + x - hor * scale, y - ver * scale, 0.0f);
            break;
      }

      // end the GL
      glEnd();

      offset += hor * 3.0f * scale;
   }
}
