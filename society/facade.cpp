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

// Change perspective from a point on the [-1, 1] scale to the corresponding cell
static float window_to_cell (
      float point, // point in the window
      int dim) // number of cells that make up the map
{
   return (point + 1.0f) / 2.0f * (float)dim;
}

/*
** function name: Facade from: Facade
*/
Facade::Facade (void)
{
   gettimeofday (&start, NULL);

   unit_positions_x = new float[10000];
   unit_positions_y = new float[10000];
   unit_positions_z = new float[10000];
   unit_selections  = new bool[10000];
   action_cells     = new int[100 * 100]; // TODO

   transform[0] = 1.0f; transform[1] = 0.0f;
   transform[2] = 0.0f; transform[3] = 1.0f;

   translation[0] = 0.0f;
   translation[1] = 0.0f;

   map_layer = 20;

   control_down = false;
   shift_down   = false;
   button0_down = false;
   button1_down = false;
   button2_down = false;
   z_down = false;

   mouse_wheel_backward_count = 0;
   mouse_wheel_forward_count = 0;

   mouse_pos[0] = 0;
   mouse_pos[1] = 0;

   selection_active = false;

   mode = 0;
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
   if (key == 27) {
      society.unselect_all ();
      mode = 0;
   }

   // Control-'a'
   else if (key == 1) {
      society.select_all ();
   }

   else if (key == 'z') {
      z_down = true;
   }

   // Dig mode
   else if (key == 'd') {
      mode = 1;
   }
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

void Facade::mouseClick (int button, int state, int x, int y)
{
   if (button == 0 && state == 0) button0_down = true;
   if (button == 1 && state == 0) button1_down = true;
   if (button == 2 && state == 0) button2_down = true;

   if (button == 0 && state == 1) button0_down = false;
   if (button == 1 && state == 1) button1_down = false;
   if (button == 2 && state == 1) button2_down = false;

   if (button == 3) mouse_wheel_forward_count++;
   if (button == 4) mouse_wheel_backward_count++;
   int window_width  = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   // Set the destination for the selected units
   if (button == 0 && state == 0 && z_down == false)
   {
      float window[2];
      window[0] = 2.0f * (float)x / (float)window_width - 1.0f;
      window[1] = 1.0f - 2.0f * (float)y / (float)window_height;

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

      int dim_x, dim_y, dim_z;
      society.access_map (&dim_x, &dim_y, &dim_z);

      float fcell[2];
      fcell[0] = (window[0] + 1.0f) / 2.0f * (float)dim_x;
      fcell[1] = (window[1] + 1.0f) / 2.0f * (float)dim_y;

      int cell[2];
      cell[0] = (int)fcell[0];
      cell[1] = (int)fcell[1];

      int destination[3];

      destination[0] = cell[0];
      destination[1] = cell[1];
      destination[2] = map_layer;

      society.set_destination (destination);
   }

   else if (mouse_wheel_forward_count >= 2 && z_down == true)
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

   else if (mouse_wheel_backward_count >= 2 && z_down == true)
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
   else if (mouse_wheel_forward_count >= 2 && z_down == false)
   {
      if (map_layer > 0) map_layer--;

      mouse_wheel_forward_count = 0;
   }

   // Increment the map layer
   else if (mouse_wheel_backward_count >= 2 && z_down == false)
   {
      int dim[3];
      society.access_map (&dim[0], &dim[1], &dim[2]);

      if (map_layer < dim[2] - 1) {
         map_layer++;
      }
      mouse_wheel_backward_count = 0;
   }
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
}

/*
**   ********
**   |  ||  |
**   |0 1 2 |
**   |      |
**   +------+
*/
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

   const float inv_transform[4] = { invDet * transform[3], -invDet * transform[1],
      -invDet * transform[2],  invDet * transform[0] };

   // Adjust the translation of the world
   if (z_down == true && button0_down == true)
   {
      translation[0] += 2.0f * (delta[0] * inv_transform[0] + delta[1] * inv_transform[1]);
      translation[1] += 2.0f * (delta[0] * inv_transform[2] + delta[1] * inv_transform[3]);
   }

   // Activate and define the selection box
   if (z_down == false && button2_down == true)
   {
      float fxt = fx;
      float fyt = fy;

      if (selection_active == false) {
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

      int dim[3];
      society.access_map (&dim[0], &dim[1], &dim[2]);

      int cell_selections[2][3];
      cell_selections[0][0] = (int)(window_to_cell (selection_box[0][0], dim[0]) + 0.5f);
      cell_selections[0][1] = (int)(window_to_cell (selection_box[0][1], dim[1]) + 0.5f);
      cell_selections[0][2] = (int)selection_box[0][2];

      cell_selections[1][0] = (int)(window_to_cell (selection_box[1][0], dim[0]) + 0.5f);
      cell_selections[1][1] = (int)(window_to_cell (selection_box[1][1], dim[1]) + 0.5f);
      cell_selections[1][2] = map_layer;

      selection_active = true;

      if (mode == 1) {
         // Dig mode
         society.select_cells (cell_selections);
      }
      else {
         society.select_units (cell_selections, map_layer, control_down);
      }
   }

   // Adjust the rotation of the world
   if (z_down == true && button1_down == true)
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
   if (z_down == true && button2_down == true)
   {
      float inv_norm_f = 1.0f / sqrtf (fx * fx + fy * fy);

      if (inv_norm_f < 0.001f) inv_norm_f = 0.001f;

      transform[1] += 2.0f * inv_norm_f * delta[0];
      transform[2] += 2.0f * inv_norm_f * delta[1];
   }

   mouse_pos[0] = x;
   mouse_pos[1] = y;
}

/*
** destructor name: ~Facade
*/
Facade::~Facade (void)
{
   delete[] unit_positions_x;
   delete[] unit_positions_y;
   delete[] unit_positions_z;
   delete[] unit_selections;
   delete[] action_cells;
}

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

   int dim_x, dim_y, dim_z;
   const float *map = society.access_map (&dim_x, &dim_y, &dim_z);
   int map_dims[3] = { dim_x, dim_y, dim_z };

   // Get digging action
   int num_dig_actions = society.get_actions (1, map_layer, action_cells);

   int num_units = society.get_unit_info (
         unit_positions_x,
         unit_positions_y,
         unit_positions_z,
         unit_selections);

   if (selection_active == true)
   {
      draw_selection_box (
            selection_box,
            transform,
            translation);
   }

   draw_actions (
         transform,
         translation,
         action_cells,
         num_dig_actions,
         map_dims,
         map_layer);

   draw_units (
         transform,
         translation,
         unit_positions_x,
         unit_positions_y,
         unit_positions_z,
         unit_selections,
         map_dims,
         num_units,
         map_layer);

   draw_map (
         transform,
         translation,
         map,
         map_dims,
         map_layer);

   hud.draw_info ();

   // swap this buffer for the old one
   glutSwapBuffers();
}
