#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "society.h"
#include "pathfinding.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

#define X_START -0.8f
#define Y_START -0.8f

#define WIDTH   0.8f
#define HEIGHT  0.8f

Society::Society (void)
{
   dim_x = 40;
   dim_y = 40;
   dim_z = 40;

   map_layer = 20;

   map = new float[dim_x * dim_y * dim_z];
   cost = new float[dim_x * dim_y * dim_z];
   buffer = new float[dim_x * dim_y * dim_z];

   for (int ind_z = 0, ind = 0; ind_z < dim_z; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
            if (rand() & 3 && ind_z == 20) map[ind] = 1.0f;
            else map[ind] = -1.0f;
//if (ind_y < dim_y / 2) map[ind] = 1.0f;
//if (ind_z != 20) map[ind] = -1.0f;
//if (ind_x == 19 && ind_y == 20) map[ind] = 1.0f;
//map[ind] = 1.0f;
         }
      }
   }

   bool found = false;

   float unit_x = 0.0f;
   float unit_y = 0.0f;
   float unit_z = 0.0f;

   for (int ind_z = 0, ind = 0; ind_z < dim_z && !found; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y && !found; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x && !found; ind_x++, ind++)
         {
            if (map[ind] > 0.0f && ind_z == map_layer) {
               found = true;
               unit_x = (float)ind_x + 0.5f;
               unit_y = (float)ind_y + 0.5f;
               unit_z = (float)map_layer + 0.5f;
            }
         }
      }
   }

   units.push_back (Unit (unit_x, unit_y, unit_z));

}

Society::~Society (void)
{
   delete[] map;
   delete[] cost;
   delete[] buffer;
}

void Society::input (Control *control)
{
   control_queue.push (control);
}

void Society::update (float time_step)
{
   static int start[3] = { 0, 0, map_layer };
   static int end[3] = { start[0], start[1], start[2] };
   static float startf[3] = { 0.0f, 0.0f, (float)map_layer + 0.5f };

   startf[0] = units[0].get_position_x ();
   startf[1] = units[0].get_position_y ();
   startf[2] = units[0].get_position_z ();

   static float dest[3] = { startf[0], startf[1], startf[2] };

   int dim[3] = { dim_x, dim_y, dim_z };

   if (control_queue.empty() == false)
   {

      Control *control = control_queue.front();

      MousePassive *mp = dynamic_cast<MousePassive*>(control);
      if (mp != 0) {
         std::cout << "PASSIVE MOTION" << std::endl;
      }

      KeyboardUp *ku = dynamic_cast<KeyboardUp*>(control);
      if (ku != 0) {
         std::cout << "KEYBOARD UP" << std::endl;
      }

      KeyboardDown *kd = dynamic_cast<KeyboardDown*>(control);
      if (kd != 0) {
         std::cout << "KEYBOARD DOWN" << std::endl;
      }

      MouseClick *mc = dynamic_cast<MouseClick*>(control);
      if (mc != 0) {
         std::cout << "MOUSE CLICK" << std::endl;
         
         int button = mc->get_button ();
         int state  = mc->get_state ();
         int x      = mc->get_x ();
         int y      = mc->get_y ();

         int row_max = dim_y;
         int col_max = dim_x;

         int window_width  = glutGet (GLUT_WINDOW_WIDTH);
         int window_height = glutGet (GLUT_WINDOW_HEIGHT);

         float block_height = HEIGHT / static_cast<float>(row_max / 2);
         float block_width  = WIDTH  / static_cast<float>(col_max / 2);

         if (button == 0 && state == 0)
         {
            float window_x = (float)x / (float)window_width;
            float window_y = (float)y / (float)window_height;

            int block_x = (int)((window_x - 0.1f) / (0.9f - 0.1f) * (float)dim_x);
            int block_y = (int)((float)dim_y - ((window_y - 0.1f) / (0.9f - 0.1f) * (float)dim_y));

            end[0] = block_x;
            end[1] = block_y;
            end[2] = map_layer;

         }
      }

      control_queue.pop();
   }

start[0] = startf[0];// + 0.5f;
start[1] = startf[1];// + 0.5f;
start[2] = startf[2];// + 0.5f;

   bool solution_found = cost_function (
         map,
         cost,
         dim,
         end,
         start,
         buffer);

   int *path = (int*)buffer;

   int path_size = pathfinding (
         cost,
         dim,
         start,
         end,
         path);

   int x_block = (path[0] % dim_x);
   int y_block = (path[0] % (dim_x * dim_y)) / dim_x;
   int z_block = path[0] / (dim_x * dim_y);

   float pos_x = units[0].get_position_x();
   float pos_y = units[0].get_position_y();
   float pos_z = units[0].get_position_z();

   float direction = 0.0f;

   units[0].set_speed (4.0f);

   float dist2 =
      (pos_x - dest[0]) * (pos_x - dest[0]) +
      (pos_y - dest[1]) * (pos_y - dest[1]) +
      (pos_z - dest[2]) * (pos_z - dest[2]);

   if ( dist2 < 0.01f) {
      if (start[0] != end[0] || start[1] != end[1] || start[2] != end[2])
      {
         dest[0] = (float)x_block + 0.5f;
         dest[1] = (float)y_block + 0.5f;
         dest[2] = (float)z_block + 0.5f;
      }
      else {
         units[0].set_speed (0.0f);
      }
   }

      if (dest[0] > pos_x) {
         if (dest[1] > pos_y) {
            direction = 0.785f;
         }
         else if (dest[1] < pos_y) {
            direction = 5.498f;
         }
         else {
            direction = 0.0f;
         }
      }
      else if (dest[0] < pos_x) {
         if (dest[1] > pos_y) {
            direction = 2.356f;
         }
         else if (dest[1] < pos_y) {
            direction = 3.927f;
         }
         else {
            direction = 3.142f;
         }
      }
      else {
         if (dest[1] > pos_y) {
            direction = 1.571f;
         }
         else if (dest[1] < pos_y) {
            direction = 4.712f;
         }
         else {
            units[0].set_speed (0.0f);
         }
      }

   units[0].move (time_step, direction);

}

const float *Society::access_map (int *dim_x_out, int *dim_y_out, int *dim_z_out)
{
   *dim_x_out = dim_x;
   *dim_y_out = dim_y;
   *dim_z_out = dim_z;
   return static_cast<const float*>(map);
}

int Society::get_unit_positions (float *x, float *y, float *z)
{
   int ind = 0;
   for (std::vector<Unit>::iterator it = units.begin(); it != units.end(); it++, ind++) {
      x[ind] = it->get_position_x();
      y[ind] = it->get_position_y();
      z[ind] = it->get_position_z();
   }

   return ind;
}
