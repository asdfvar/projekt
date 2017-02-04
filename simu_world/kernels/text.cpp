#include "text.h"
#include <iostream>
#include <stdio.h>
#include "opengl_interface.h"
#include <sstream>
#include <cstdlib>

/*         -------> +x
**    (0,0)    1         2
**        **********************
**  +y    **********************
**   ^    ***       **       ***
**   |    ** *      **      * **
**   |    **  *   5 **     *  **
**   |    **   *    **  6 *   **
**   |  3 **  4 *   **   *  7 **
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
         glVertex3f(x,                      y,               0.0f);
         glVertex3f(x + hor * scale,        y,               0.0f);
        break;

      case 2:
         glVertex3f(x +        hor * scale, y,               0.0f);
         glVertex3f(x + 2.0f * hor * scale, y,               0.0f);
        break;

      case 3:
         glVertex3f(x,                      y,               0.0f);
         glVertex3f(x,                      y - ver * scale, 0.0f);
        break;

      case 4:
         glVertex3f(x,                      y,               0.0f);
         glVertex3f(x + hor * scale,        y - ver * scale, 0.0f);
        break;

      case 5:
         glVertex3f(x + hor * scale, y - ver * scale + ver * scale, 0.0f);
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
        break;

      case 6:
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale + ver * scale, 0.0f);
        break;

      case 7:
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale + ver * scale, 0.0f);
        break;

      case 8:
         glVertex3f(x + hor * scale - hor * scale, y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
        break;

      case 9:
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale,               0.0f);
        break;

      case 10:
         glVertex3f(x + hor * scale - hor * scale, y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale - hor * scale, y - ver * scale - ver * scale, 0.0f);
        break;

      case 11:
         glVertex3f(x + hor * scale - hor * scale, y - ver * scale - ver * scale, 0.0f);
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
        break;

      case 12:
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale,               y - ver * scale - ver * scale, 0.0f);
        break;

      case 13:
         glVertex3f(x + hor * scale,               y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale - ver * scale, 0.0f);
        break;

      case 14:
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale - ver * scale, 0.0f);
        break;

      case 15:
         glVertex3f(x + hor * scale - hor * scale, y - ver * scale - ver * scale, 0.0f);
         glVertex3f(x + hor * scale,               y - ver * scale - ver * scale, 0.0f);
        break;

      case 16:
         glVertex3f(x + hor * scale,               y - ver * scale - ver * scale, 0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale - ver * scale, 0.0f);
        break;

      case 17:
         glVertex3f(x + hor * scale,               y - ver * scale - ver * scale, 0.0f);
         glVertex3f(x + hor * scale + hor * scale, y - ver * scale,               0.0f);
        break;

      case 18:
         glVertex3f(x + hor * scale - hor * scale, y - ver * scale,               0.0f);
         glVertex3f(x + hor * scale,               y - ver * scale - ver * scale, 0.0f);
        break;

      case 19:
         glVertex3f(x - 0.002f * scale, y - (2.0f * ver - 0.002f) * scale,   0.0f);
         glVertex3f(x + 0.002f * scale, y - (2.0f * ver - 0.002f) * scale,   0.0f);
         glVertex3f(x + 0.002f * scale, y - (2.0f * ver  * scale),           0.0f);
         glVertex3f(x - 0.002f * scale, y - (2.0f * ver  * scale),           0.0f);
         glVertex3f(x - 0.002f * scale, y - (2.0f * ver - 0.002f) * scale,   0.0f);
        break;

      case 20:
         glVertex3f(x + 0.006f * scale, y - (2.0f * ver - 0.006f) * scale,   0.0f);
         glVertex3f(x - 0.004f * scale, y - (2.0f * ver + 0.004f) * scale,   0.0f);
        break;
   }
}

Text::Text( void )
{
   row = 0;
   curr_buffer = buffer_1;
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
      glColor3f( 0.0f, 0.8f, 0.0f );
      glLineWidth( 3.0f );
      glBegin( GL_LINES );

      switch( std::toupper(input.at(k)) )
      {
         case 'A':
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'B':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  5, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'C':
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'D':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  5, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'E':
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'F':
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'G':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'H':
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'I':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  5, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'J':
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'K':
            LCD_font(  5, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  6, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 13, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'L':
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'M':
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  4, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  6, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'N':
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  4, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 13, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'O':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'P':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'Q':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 13, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'R':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 13, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'S':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'T':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  5, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'U':
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'V':
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 17, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 18, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'W':
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'X':
            LCD_font(  4, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 13, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  6, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 11, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'Y':
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  6, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  4, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case 'Z':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  6, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 11, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '1':
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '2':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '3':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '4':
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '5':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '6':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '7':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '8':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '9':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '0':
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case ' ':
            break;

         case '.':
            LCD_font( 19, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case ',':
            LCD_font( 20, offset + x + 0.01f, y - 0.01f, scale);
            break;

         case '-':
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            break;

         default:
            LCD_font(  1, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  2, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  3, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  4, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  5, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  6, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  7, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  8, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font(  9, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 10, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 11, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 12, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 13, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 14, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 15, offset + x + 0.01f, y - 0.01f, scale);
            LCD_font( 16, offset + x + 0.01f, y - 0.01f, scale);
            break;
      }

      // end the GL
      glEnd();

      offset += hor * 3.0f * scale;
   }

   offset = 0.0f;

   for (int k = 0; k < input.length(); k++)
   {
      float vertices_x[4] = { offset + x + 0.01f - hor * 0.5f * scale,
                              offset + x + 0.01f + hor * 2.8f * scale,
                              offset + x + 0.01f + hor * 2.8f * scale,
                              offset + x + 0.01f - hor * 0.5f * scale };

      float vertices_y[4] = { y - ver * 2.8f * scale,
                              y - ver * 2.8f * scale,
                              y + ver * 0.5f * scale,
                              y + ver * 0.5f * scale };

      float color[3]     = { 0.0f, 0.0f, 0.0f };

      ogl::draw_2d_polygon( vertices_x,
                            vertices_y,
                            color,
                            4);

      offset += hor * 3.0f * scale;
   }
}

void Text::new_line( void )
{
   row++;
}

void Text::clear( void )
{
   for (int k = 0; k <= row; k++)
      curr_buffer[k].clear();

   row = 0;
}

void Text::populate( std::string input )
{
   curr_buffer[row] += input;
}

void Text::populate( int number )
{
   std::ostringstream id_str;
   id_str << number;
   curr_buffer[row] += id_str.str();
}

void Text::populate( float number )
{
   std::ostringstream id_str;
   id_str << number;
   curr_buffer[row] += id_str.str();
}

void Text::display_contents( const float x,
                             const float y,
                             const float scale )
{

   float ver = 0.02;

   for (int k = 0; k <= row; k++)
   {
      write_to_screen( curr_buffer[k],
                       x,
                       y - (float)k * ver * 3.0f * scale,
                       scale );
   }

   // swap buffers
   if (curr_buffer == buffer_1) curr_buffer = buffer_2;
   if (curr_buffer == buffer_2) curr_buffer = buffer_1;
}
