#include "text.h"
#include <iostream>
#include <stdio.h>
#include "opengl_interface.h"

/*             1         2
**        **********************
**        **********************
**        ***       **       ***
**        ** *      **      * **
**        **  *   5 **     *  **
**        **   *    **  6 *   **
**      3 **  4 *   **   *  7 **
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

      case 10:
         glVertex3f(x - hor * scale, y,               0.0f);
         glVertex3f(x - hor * scale, y - ver * scale, 0.0f);
        break;

      case 11:
         glVertex3f(x - hor * scale, y - ver * scale, 0.0f);
         glVertex3f(x,               y,               0.0f);
        break;

      case 12:
         glVertex3f(x,               y,               0.0f);
         glVertex3f(x,               y - ver * scale, 0.0f);
        break;

      case 13:
         glVertex3f(x,               y,               0.0f);
         glVertex3f(x + hor * scale, y - ver * scale, 0.0f);
        break;

      case 14:
         glVertex3f(x + hor * scale, y,               0.0f);
         glVertex3f(x + hor * scale, y - ver * scale, 0.0f);
        break;

      case 15:
         glVertex3f(x - hor * scale, y - ver * scale, 0.0f);
         glVertex3f(x,               y - ver * scale, 0.0f);
        break;

      case 16:
         glVertex3f(x,               y - ver * scale, 0.0f);
         glVertex3f(x + hor * scale, y - ver * scale, 0.0f);
        break;

      case 17:
         glVertex3f(x,               y - ver * scale, 0.0f);
         glVertex3f(x + hor * scale, y,               0.0f);
        break;

      case 18:
         glVertex3f(x - hor * scale, y,               0.0f);
         glVertex3f(x,               y - ver * scale, 0.0f);
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
      glColor3f( 0.0f, 0.6f, 0.0f );
      glLineWidth( 3.0f );
      glBegin( GL_LINES );

      switch( input.at(k) )
      {
         case 'A':
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            break;

         case 'B':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            LCD_font(  5, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            break;

         case 'C':
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            break;

         case 'D':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            LCD_font(  5, offset + x, y, scale);
            break;

         case 'E':
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            break;

         case 'F':
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            break;

         case 'G':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            break;

         case 'H':
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            break;

         case 'I':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font(  5, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            break;

         case 'J':
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            break;

         case 'K':
            LCD_font(  5, offset + x, y, scale);
            LCD_font(  6, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            LCD_font( 13, offset + x, y, scale);
            break;

         case 'L':
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case 'M':
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  4, offset + x, y, scale);
            LCD_font(  6, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            break;

         case 'N':
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  4, offset + x, y, scale);
            LCD_font( 13, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            break;

         case 'O':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case 'P':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            break;

         case 'Q':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 13, offset + x, y, scale);
            break;

         case 'R':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 13, offset + x, y, scale);
            break;

         case 'S':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case 'T':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  5, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            break;

         case 'U':
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            break;

         case 'V':
            LCD_font(  7, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 17, offset + x, y, scale);
            LCD_font( 18, offset + x, y, scale);
            break;

         case 'W':
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            break;

         case 'X':
            LCD_font(  4, offset + x, y, scale);
            LCD_font( 13, offset + x, y, scale);
            LCD_font(  6, offset + x, y, scale);
            LCD_font( 11, offset + x, y, scale);
            break;

         case 'Y':
            LCD_font( 12, offset + x, y, scale);
            LCD_font(  6, offset + x, y, scale);
            LCD_font(  4, offset + x, y, scale);
            break;

         case 'Z':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  6, offset + x, y, scale);
            LCD_font( 11, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case '1':
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            break;

         case '2':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case '3':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            break;

         case '4':
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            break;

         case '5':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case '6':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            break;

         case '7':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            break;

         case '8':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case '9':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         case '0':
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;

         default:
            LCD_font(  1, offset + x, y, scale);
            LCD_font(  2, offset + x, y, scale);
            LCD_font(  3, offset + x, y, scale);
            LCD_font(  4, offset + x, y, scale);
            LCD_font(  5, offset + x, y, scale);
            LCD_font(  6, offset + x, y, scale);
            LCD_font(  7, offset + x, y, scale);
            LCD_font(  8, offset + x, y, scale);
            LCD_font(  9, offset + x, y, scale);
            LCD_font( 10, offset + x, y, scale);
            LCD_font( 11, offset + x, y, scale);
            LCD_font( 12, offset + x, y, scale);
            LCD_font( 13, offset + x, y, scale);
            LCD_font( 14, offset + x, y, scale);
            LCD_font( 15, offset + x, y, scale);
            LCD_font( 16, offset + x, y, scale);
            break;
      }

      // end the GL
      glEnd();

      offset += hor * 3.0f * scale;
   }

   offset = 0.0f;

   for (int k = 0; k < input.length(); k++)
   {
      float vertices_x[4] = { offset + x - hor * 1.5f,
                              offset + x + hor * 1.5f,
                              offset + x + hor * 1.5f,
                              offset + x - hor * 1.5f };

      float vertices_y[4] = { y - ver * 1.5f,
                              y - ver * 1.5f,
                              y + ver * 1.5f,
                              y + ver * 1.5f };

      float color[3]     = { 0.0f, 0.0f, 0.0f };

      ogl::draw_2d_polygon( vertices_x,
                            vertices_y,
                            color,
                            4);

      offset += hor * 3.0f * scale;
   }
}
