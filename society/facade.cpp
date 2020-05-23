#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <cmath>
#include "facade.h"
#include "graphics.h"
#include "mode.h"
#include "utils.h"

/*
** function name: Facade from: Facade
*/
Facade::Facade (void)
{
   gettimeofday (&start, NULL);

   active_menu = nullptr;

   transform[0] = 1.0f; transform[1] = 0.0f;
   transform[2] = 0.0f; transform[3] = 1.0f;

   translation[0] = 0.0f;
   translation[1] = 0.0f;

   map_layer = 20;

   society.set_map_layer (map_layer);

   society.set_view (transform, translation);

   control_down = false;
   shift_down   = false;
   button0_down = false;
   button1_down = false;
   button2_down = false;
   z_down       = false;
   has_moved_passively    = false;

   mouse_wheel_backward_count = 0;
   mouse_wheel_forward_count = 0;

   mouse_pos[0] = 0;
   mouse_pos[1] = 0;

   selection_active = false;

   mode = mode::NONE;
}

/*
** function name: keyboardUp from: Facade
*/
void Facade::keyboardUp (const char key, int x, int y)
{
   if (key == 'z') {
      z_down = false;
   }
}

/*
** function name: keyboardDown from: Facade
** glutGetModifiers() is true if the shift key is depressed
*/
void Facade::keyboardDown (const char key, int x, int y)
{
   // Escape key
   if (!control_down && key == 27) {
      society.unselect_all ();
      mode        = mode::NONE;

      if (active_menu != nullptr)
         active_menu->reset ();

      active_menu = nullptr;
   }

   // Enter key
   if (!control_down && key == 13)
   {
      if (mode == mode::REMOVE) society.set_jobs (jid::REMOVE, 0);
      if (mode == mode::BUILD ) society.set_jobs (jid::BUILD, jobmaterial);

      mode = mode::NONE;
   }

   // Control 'a'
   else if (control_down && key == 1) {
      society.select_all ();
   }

   else if (key == 'z') {
      z_down = true;
   }

   // Control 1
   else if (control_down && key == 49)
      society.set_group (1);

   // Control 2
   else if (control_down && key == 0)
      society.set_group (2);

   // Control 3
   else if (control_down && key == 27)
      society.set_group (3);

   // Control 4
   else if (control_down && key == 28)
      society.set_group (4);

   // Control 5
   else if (control_down && key == 29)
      society.set_group (5);

   // Control 6
   else if (control_down && key == 30)
      society.set_group (6);

   // Control 7
   else if (control_down && key == 31)
      society.set_group (7);

   // Control 8
   else if (control_down && key == 127)
      society.set_group (8);

   // Control 9
   else if (control_down && key == 57)
      society.set_group (9);

   // Control 0
   else if (control_down && key == 48)
      society.set_group (0);

   // Shift 1
   else if (shift_down && key == 33)
      society.select_group (1);

   // Shift 2
   else if (shift_down && key == 64)
      society.select_group (2);

   // Shift 3
   else if (shift_down && key == 35)
      society.select_group (3);

   // Shift 4
   else if (shift_down && key == 36)
      society.select_group (4);

   // Shift 5
   else if (shift_down && key == 37)
      society.select_group (5);

   // Shift 6
   else if (shift_down && key == 94)
      society.select_group (6);

   // Shift 7
   else if (shift_down && key == 38)
      society.select_group (7);

   // Shift 8
   else if (shift_down && key == 42)
      society.select_group (8);

   // Shift 9
   else if (shift_down && key == 40)
      society.select_group (9);

   // Shift 0
   else if (shift_down && key == 41)
      society.select_group (0);

   else if (key == 'm') {
      active_menu = &main_menu;
   }

   hud.set_mode (mode);
}

/*
** function name: specialFunc from: Facade
*/
void Facade::specialFunc (int key, int x, int y)
{
   // Control key
   if (key == 114) {
      control_down = true;
   }

   // Shift key
   else if (key == 112) {
      shift_down = true;
   }
}

/*
** function name: specialUpFunc from: Facade
*/
void Facade::specialUpFunc (int key, int x, int y)
{
   // Control key
   if (key == 114) {
      control_down = false;
   }

   // Shift key
   else if (key == 112) {
      shift_down = false;
   }
}

/*
**   ********
**   |  ||  |
**   |0 1 2 |
**   |      |
**   +------+
*/
void Facade::mouseClick (int button, int state, int x, int y)
{
   if (button == 0 && state == 0) button0_down = true;
   if (button == 1 && state == 0) button1_down = true;
   if (button == 2 && state == 0) button2_down = true;

   if (button == 0 && state == 1) button0_down = false;
   if (button == 1 && state == 1) button1_down = false;
   if (button == 2 && state == 1) button2_down = false;

   if (button == 3 && state == 0) mouse_wheel_forward_count++;
   if (button == 4 && state == 0) mouse_wheel_backward_count++;

   int window_width  = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   float window[2];
   window[0] =        2.0f * (float)x / (float)window_width - 1.0f;
   window[1] = 1.0f - 2.0f * (float)y / (float)window_height;

   if (button0_down == false)
   {
      if (active_menu != nullptr)
      {
         // "main" menu
         if (active_menu->get_menu_id () == 1)
         {
            // "Remove" button
            if (active_menu->lunclick (window[0], window[1]) == 1)
            {
               mode = mode::REMOVE;
               active_menu = nullptr;
               hud.set_mode (mode);
            }

            // "Build" button
            else if (active_menu->lunclick (window[0], window[1]) == 2)
            {
               active_menu->reset ();
               active_menu = &build_menu;
               hud.set_mode (mode);
            }
         }

         // "build" menu
         else if (active_menu->get_menu_id () == 2)
         {
            // "material" button
            if (active_menu->lunclick (window[0], window[1]) == 1)
            {
               active_menu = &build_material_menu;
            }
         }

         // "build material" menu
         else if (active_menu->get_menu_id () == 3)
         {
            // "Dirt" button
            if (active_menu->lunclick (window[0], window[1]) == 1)
            {
               mode        = mode::BUILD;
               jobmaterial = mid::dirt;
               active_menu = nullptr;
               hud.set_mode (mode);
            }

            // "Stone" button
            else if (active_menu->lunclick (window[0], window[1]) == 2)
            {
               mode        = mode::BUILD;
               jobmaterial = mid::stone;
               active_menu = nullptr;
               hud.set_mode (mode);
            }
         }
      }
   }

   if (button0_down == true)
   {
      if (active_menu != nullptr) active_menu->lclick (window[0], window[1]);
   }

   // Set the destination for the selected units
   if (button == 0 && state == 1 && z_down == false && has_moved_passively == true)
   {
      const float det = transform[0] * transform[3] - transform[1] * transform[2];
      const float invDet = 1.0f / det;

      const float inv_transform[4] = { invDet * transform[3], -invDet * transform[1],
         -invDet * transform[2],  invDet * transform[0] };

      float temp = window[0];

      window[0] =
         window[0] * inv_transform[0] +
         window[1] * inv_transform[1] - translation[0];

      window[1] =
         temp      * inv_transform[2] +
         window[1] * inv_transform[3] - translation[1];

      int size[2] = {
         society.get_size (0),
         society.get_size (1) };

      float fcell[2];
      fcell[0] = (window[0] + 1.0f) / 2.0f * (float)size[0];
      fcell[1] = (window[1] + 1.0f) / 2.0f * (float)size[1];

      int cell[2];
      cell[0] = (int)fcell[0];
      cell[1] = (int)fcell[1];

      int destination[3];

      destination[0] = cell[0];
      destination[1] = cell[1];
      destination[2] = map_layer;

      society.set_destination (destination, true);
   }

   // Zoom in
   else if (mouse_wheel_forward_count > 0 && z_down == false)
   {
      if (transform[0] <= 10.0f && transform[3] <= 10.0f)
      {
         transform[0] *= 1.1f;
         transform[1] *= 1.1f;
         transform[2] *= 1.1f;
         transform[3] *= 1.1f;
      }

      mouse_wheel_forward_count = 0;
   }

   // Zoom out
   else if (mouse_wheel_backward_count > 0 && z_down == false)
   {
      if (transform[0] >= 0.1f && transform[3] >= 0.1f) {
         transform[0] *= 0.9f;
         transform[1] *= 0.9f;
         transform[2] *= 0.9f;
         transform[3] *= 0.9f;
      }

      mouse_wheel_backward_count = 0;
   }

   // Decrement the map layer
   else if (mouse_wheel_forward_count > 0 && z_down == true)
   {
      if (map_layer > 0) map_layer--;

      mouse_wheel_forward_count = 0;
   }

   // Increment the map layer
   else if (mouse_wheel_backward_count > 0 && z_down == true)
   {
      if (map_layer < society.get_size (2) - 1) map_layer++;

      mouse_wheel_backward_count = 0;
   }

   int size[2] = {
      society.get_size (0),
      society.get_size (1) };

   int cell_selections[2][3];
   cell_selections[0][0] = (int)(window_to_cell (selection_box[0][0], size[0]) + 0.5f);
   cell_selections[0][1] = (int)(window_to_cell (selection_box[0][1], size[1]) + 0.5f);
   cell_selections[0][2] = (int)selection_box[0][2];

   cell_selections[1][0] = (int)(window_to_cell (selection_box[1][0], size[0]) + 0.5f);
   cell_selections[1][1] = (int)(window_to_cell (selection_box[1][1], size[1]) + 0.5f);
   cell_selections[1][2] = map_layer;

   society.select_cells (cell_selections);
   if (mode == mode::REMOVE)
   {
      if (button2_down == false)
         society.set_select_cells (!control_down, mode::REMOVE);
   }
   else if (mode == mode::BUILD)
   {
      if (button2_down == false)
         society.set_select_cells (!control_down, mode::BUILD);
   }

   society.set_map_layer (map_layer);
}

/*
 ** function name: mousePassive from Facade
 **
 ** (0,0) is defined in the top left corner of the window
 ** x is the column number of cells from left
 ** y is the row number of cells from top
 */
void Facade::mousePassive (int x, int y)
{
   mouse_pos[0] = x;
   mouse_pos[1] = y;

   selection_active = false;

   has_moved_passively = true;
}

void Facade::mouseMotion (int x, int y)
{
   int window_width  = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   float fx = (float)(2 * x - window_width) / (float)window_width;
   float fy = (float)(window_height - 2 * y) / (float)window_height;

   int diff[2] = { x - mouse_pos[0], mouse_pos[1] - y };

   float delta[2];
   delta[0] = (float)diff[0] / (float)window_width;
   delta[1] = (float)diff[1] / (float)window_height;

   const float det = transform[0] * transform[3] - transform[1] * transform[2];
   const float invDet = 1.0f / det;

   const float inv_transform[4] = {
      invDet * transform[3], -invDet * transform[1],
      -invDet * transform[2],  invDet * transform[0] };

   // Adjust the translation of the world
   if (z_down == false && button0_down == true)
   {
      if (active_menu != nullptr)
      {
         active_menu->translate (2.0f * delta[0], 2.0f * delta[1]);
      }
      else {
         translation[0] += 2.0f * (delta[0] * inv_transform[0] + delta[1] * inv_transform[1]);
         translation[1] += 2.0f * (delta[0] * inv_transform[2] + delta[1] * inv_transform[3]);
      }
   }

   // Activate and define the selection box
   else if (z_down == false && button2_down == true)
   {
      float fxt = fx;
      float fyt = fy;

      if (selection_active == false)
      {
         selection_box[0][0] = fxt * inv_transform[0] + fyt * inv_transform[1];
         selection_box[0][1] = fxt * inv_transform[2] + fyt * inv_transform[3];
         selection_box[0][2] = (float)map_layer + 0.5f;

         selection_box[0][0] -= translation[0];
         selection_box[0][1] -= translation[1];
      }

      selection_box[1][0] = fxt * inv_transform[0] + fyt * inv_transform[1];
      selection_box[1][1] = fxt * inv_transform[2] + fyt * inv_transform[3];
      selection_box[1][2] = (float)map_layer + 0.5f;

      selection_box[1][0] -= translation[0];
      selection_box[1][1] -= translation[1];

      int size[2] = {
         society.get_size (0),
         society.get_size (1) };

      int cell_selections[2][3];
      cell_selections[0][0] = (int)(window_to_cell (selection_box[0][0], size[0]) + 0.5f);
      cell_selections[0][1] = (int)(window_to_cell (selection_box[0][1], size[1]) + 0.5f);
      cell_selections[0][2] = (int)selection_box[0][2];

      cell_selections[1][0] = (int)(window_to_cell (selection_box[1][0], size[0]) + 0.5f);
      cell_selections[1][1] = (int)(window_to_cell (selection_box[1][1], size[1]) + 0.5f);
      cell_selections[1][2] = map_layer;

      selection_active = true;

      if (mode == mode::REMOVE)
      {
         society.select_cells (cell_selections);
      }
      else
      {
         society.select_units (cell_selections, map_layer, control_down);
      }
   }

   // Adjust the rotation of the world
   else if (z_down == true && button1_down == true)
   {
      float norm_f2   = fx * fx + fy * fy;
      float norm_d2   = delta[0] * delta[0] + delta[1] * delta[1];
      float dot_prod  = fx * delta[0] + fy * delta[1];
      float ortho_mag = norm_d2 - dot_prod * dot_prod / norm_f2;

      float norm2 = norm_f2 + ortho_mag * ortho_mag;

      float costh = sqrtf (norm_f2   / norm2);
      float sinth = sqrtf (ortho_mag / norm2);

      if (norm2 < 0.001f) costh = 1.0f;
      if (norm2 < 0.001f) sinth = 0.0f;

      float cross_prod = fx * delta[1] - fy * delta[0];
      if (cross_prod < 0.0f) sinth = -sinth;

      float temp[4];

      temp[0] = transform[0] * costh    + transform[1] * sinth;
      temp[1] = transform[0] * (-sinth) + transform[1] * costh;
      temp[2] = transform[2] * costh    + transform[3] * sinth;
      temp[3] = transform[2] * (-sinth) + transform[3] * costh;

      transform[0] = temp[0];
      transform[1] = temp[1];
      transform[2] = temp[2];
      transform[3] = temp[3];
   }

   // Shear the world map
   else if (z_down == true && button2_down == true)
   {
      float inv_norm_f = 1.0f / sqrtf (fx * fx + fy * fy);

      if (inv_norm_f < 0.001f) inv_norm_f = 0.001f;

      transform[1] += 2.0f * inv_norm_f * delta[0];
      transform[2] += 2.0f * inv_norm_f * delta[1];
   }

   mouse_pos[0] = x;
   mouse_pos[1] = y;

   has_moved_passively = false;
}

/*
** destructor name: ~Facade
*/
Facade::~Facade (void) { }

/*
** function name: idle from: Facade
*/
void Facade::idle (void)
{
   double time_taken;

   do {
      gettimeofday (&end, NULL);
      time_taken = (end.tv_sec * 1000000 + end.tv_usec -
                   (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;
   } while (time_taken < time_step);

   gettimeofday (&start, NULL);

   // Update society at this time step
   society.update (time_step);

   // Update HUD information
   hud.update (time_step, map_layer);

   glutPostRedisplay ();
}

/*
** function name: display from: Facade
*/
void Facade::display (void)
{
   // clear this openGL buffer
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if (selection_active == true)
   {
      graphics.draw_selection_box (
            selection_box,
            transform,
            translation);
   }

   society.draw (
         transform,
         translation,
         map_layer);

   hud.draw_info ();

   if (active_menu != nullptr) {
      active_menu->show ();
   }

   // swap this buffer for the old one
   glutSwapBuffers();
}
