#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "text.h"
#include <iostream>
#include <stdio.h>
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
static void LCD_font (
      unsigned int plick,
      const float  x,
      const float  y,
      const float  scale)
{
   const float hor = 0.01f;
   const float ver = 0.02f;

   const float depth = -0.5f;

   switch (plick)
   {
      case 1:
         glVertex3f (x,                      y,                      depth);
         glVertex3f (x + hor * scale,        y,                      depth);
        break;

      case 2:
         glVertex3f (x +        hor * scale, y,                      depth);
         glVertex3f (x + 2.0f * hor * scale, y,                      depth);
        break;

      case 3:
         glVertex3f (x,                      y,                      depth);
         glVertex3f (x,                      y - ver * scale,        depth);
        break;

      case 4:
         glVertex3f (x,                      y,                      depth);
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
        break;

      case 5:
         glVertex3f (x + hor * scale,        y,                      depth);
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
        break;

      case 6:
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
         glVertex3f (x + 2.0f * hor * scale, y,                      depth);
        break;

      case 7:
         glVertex3f (x + 2.0f * hor * scale, y - ver * scale,        depth);
         glVertex3f (x + 2.0f * hor * scale, y,                      depth);
        break;

      case 8:
         glVertex3f (x,                      y - ver * scale,        depth);
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
        break;

      case 9:
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
         glVertex3f (x + 2.0f * hor * scale, y - ver * scale,        depth);
        break;

      case 10:
         glVertex3f (x,                      y - ver * scale,        depth);
         glVertex3f (x,                      y - 2.0f * ver * scale, depth);
        break;

      case 11:
         glVertex3f (x,                      y - 2.0f * ver * scale, depth);
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
        break;

      case 12:
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
         glVertex3f (x + hor * scale,        y - 2.0f * ver * scale, depth);
        break;

      case 13:
         glVertex3f (x + hor * scale,        y - ver * scale,        depth);
         glVertex3f (x + 2.0f * hor * scale, y - 2.0f * ver * scale, depth);
        break;

      case 14:
         glVertex3f (x + 2.0f * hor * scale, y - ver * scale,        depth);
         glVertex3f (x + 2.0f * hor * scale, y - 2.0f * ver * scale, depth);
        break;

      case 15:
         glVertex3f (x,                      y - 2.0f * ver * scale, depth);
         glVertex3f (x + hor * scale,        y - 2.0f * ver * scale, depth);
        break;

      case 16:
         glVertex3f (x + hor * scale,        y - 2.0f * ver * scale, depth);
         glVertex3f (x + 2.0f * hor * scale, y - 2.0f * ver * scale, depth);
        break;

      // SPECIAL CHARACTERS

      // right side of the "v" character's slope
      case 17:
         glVertex3f (x + hor * scale,        y - 2.0f * ver * scale, depth);
         glVertex3f (x + 2.0f * hor * scale, y - ver * scale,        depth);
        break;

      // left side of the "v" character's slope
      case 18:
         glVertex3f (x,                      y - ver * scale,        depth);
         glVertex3f (x + hor * scale,        y - 2.0f * ver * scale, depth);
        break;

      // "." period
      case 19:
         glVertex3f (x - 0.002f * scale, y - (2.0f * ver - 0.002f) * scale,   depth);
         glVertex3f (x + 0.002f * scale, y - (2.0f * ver - 0.002f) * scale,   depth);
         glVertex3f (x + 0.002f * scale, y - (2.0f * ver * scale),            depth);
         glVertex3f (x - 0.002f * scale, y - (2.0f * ver * scale),            depth);
         glVertex3f (x - 0.002f * scale, y - (2.0f * ver - 0.002f) * scale,   depth);
        break;

      // "," comma
      case 20:
         glVertex3f (x + 0.006f * scale, y - (2.0f * ver - 0.006f) * scale,   depth);
         glVertex3f (x - 0.004f * scale, y - (2.0f * ver + 0.004f) * scale,   depth);
        break;

      // ":" colon
      case 21:
         glVertex3f (x + hor * scale, y - 0.333f * ver * scale,   depth);
         glVertex3f (x + hor * scale, y - 0.666f * ver * scale,   depth);
         glVertex3f (x + hor * scale, y - 1.333f * ver * scale,   depth);
         glVertex3f (x + hor * scale, y - 1.666f * ver * scale,   depth);
        break;

      // "=" equal
      case 22:
         glVertex3f (x + 0.25f * hor * scale, y - 0.75f * ver * scale,   depth);
         glVertex3f (x + 1.75f * hor * scale, y - 0.75f * ver * scale,   depth);
         glVertex3f (x + 0.25f * hor * scale, y - 1.25f * ver * scale,   depth);
         glVertex3f (x + 1.75f * hor * scale, y - 1.25f * ver * scale,   depth);
        break;
   }
}

Text::Text (void)
{
   row = 0;
}

/*
** function name: write_to_screen from: Text
*/
void Text::write_to_screen (
      std::string input,
      const float x,
      const float y,
      const float alpha,
      const float scale)
{
   const float hor = 0.01f;
   const float ver = 0.02f;

   float offset = 0.0f;

   /*
   ** draw the background for contrasting the text
   */
   for (int k = 0; k < input.length(); k++)
   {
      float vertices_x[4] = {
         offset + x + 0.01f - hor * 0.5f * scale,
         offset + x + 0.01f + hor * 2.8f * scale,
         offset + x + 0.01f + hor * 2.8f * scale,
         offset + x + 0.01f - hor * 0.5f * scale };

      float vertices_y[4] = {
         y - ver * 2.8f * scale,
         y - ver * 2.8f * scale,
         y + ver * 0.5f * scale,
         y + ver * 0.5f * scale };

      float color[3] = { 0.0f, 0.0f, 0.0f };

      // setup the GL conditions
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f (color[0], color[1], color[2], alpha);
      glBegin (GL_POLYGON);

      for (int vertex = 0; vertex < 4; vertex++)
      {
         glVertex3f (vertices_x[vertex], vertices_y[vertex], -0.4f);
      }

      glEnd();

      offset += hor * 3.0f * scale;
   }

   offset = 0.0f;

   float local_vert_offset = 0.00f;
   float local_hor_offset  = 0.01f;

   for (int k = 0; k < input.length (); k++)
   {
      // setup the GL conditions
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f (0.0f, 0.8f, 0.0f, alpha);
      glLineWidth (3.0f);
      glBegin (GL_LINES);

      switch (std::toupper (input.at (k)))
      {
         case 'A':
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'B':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  5, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'C':
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'D':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  5, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'E':
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'F':
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'G':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'H':
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'I':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  5, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'J':
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'K':
            LCD_font (  5, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  6, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 13, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'L':
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'M':
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  4, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  6, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'N':
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  4, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 13, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'O':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'P':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'Q':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 13, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'R':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 13, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'S':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'T':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  5, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'U':
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'V':
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 17, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 18, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'W':
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'X':
            LCD_font (  4, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 13, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  6, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 11, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'Y':
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  6, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  4, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case 'Z':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  6, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 11, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '1':
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '2':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '3':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '4':
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '5':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '6':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '7':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '8':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '9':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '0':
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case ' ':
            break;

         case '.':
            LCD_font ( 19, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case ',':
            LCD_font ( 20, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '-':
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case ':':
            LCD_font ( 21, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         case '=':
            LCD_font ( 22, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;

         default:
            LCD_font (  1, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  2, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  3, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  4, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  5, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  6, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  7, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  8, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font (  9, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 10, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 11, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 12, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 13, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 14, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 15, offset + x + local_hor_offset, y + local_vert_offset, scale);
            LCD_font ( 16, offset + x + local_hor_offset, y + local_vert_offset, scale);
            break;
      }

      // end the GL
      glEnd ();

      offset += hor * 3.0f * scale;
   }
}

void Text::new_line (void)
{
   row++;
}

void Text::populate (std::string input)
{
   buffer[row] += input;
}

void Text::populate (int number)
{
   std::ostringstream id_str;
   id_str << number;
   buffer[row] += id_str.str();
}

void Text::populate (float number)
{
   std::ostringstream id_str;
   id_str << number;
   buffer[row] += id_str.str();
}

void Text::populate (double number)
{
   std::ostringstream id_str;
   id_str << number;
   buffer[row] += id_str.str();
}

/*
** function: display_contents from: Text
*/
void Text::display_contents (
      const float x,
      const float y,
      const float alpha,
      const float scale)
{
   const float ver = 0.02f;

   for (int k = 0; k <= row; k++)
   {
      write_to_screen (
            buffer[k],
            x,
            y - (float)k * ver * 3.0f * scale,
            alpha,
            scale);
   }
}
