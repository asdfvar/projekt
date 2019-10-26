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

   map = new float[dim_x * dim_y * dim_z];
   cost = new float[dim_x * dim_y * dim_z];
   buffer = new float[dim_x * dim_y * dim_z];

   for (int ind_z = 0, ind = 0; ind_z < dim_z; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
         {
            if (rand() & 3 && ind_z == 20) map[ind] = 1.0f;
            else map[ind] = -1.0f;
map[ind] = 1.0f;
         }
      }
   }

   units.push_back (Unit (10.5f, 10.5f, 20.5f));

   destination[0] = 0;
   destination[1] = 0;
   destination[2] = 20;
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
   static int start[3] = { 0, 0, 20 };
   static int end[3] = { start[0], start[1], start[2] };
   static float startf[3] = { 0.0f, 0.0f, 20.0f };

   startf[0] = units[0].get_position_x ();
   startf[1] = units[0].get_position_y ();
   startf[2] = units[0].get_position_z ();

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
            int block_y = dim_y - (int)((window_y - 0.1f) / (0.9f - 0.1f) * (float)dim_y);

            end[0] = block_x;
            end[1] = block_y;
            end[2] = 20;

std::cout << "start = " << start[0] << ", " << start[1] << ", " << start[2] << std::endl;
std::cout << "end   = " << end[0] << ", " << end[1] << ", " << end[2] << std::endl;

         }
      }

      control_queue.pop();
   }

start[0] = startf[0] + 0.5f;
start[1] = startf[1] + 0.5f;
start[2] = startf[2] + 0.5f;

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

std::cout << "path forecast: ";
for (int k = 0; k < 4; k++) {
   int x_block = (path[k] % dim_x);
   int y_block = (path[k] % (dim_x * dim_y)) / dim_x;
   std::cout << "(" << x_block << ", " << y_block << "), ";
}
std::cout << std::endl;
std::cout << "end   = " << end[0] << ", " << end[1] << ", " << end[2] << std::endl;

   int x_block = (path[0] % dim_x);
   int y_block = (path[0] % (dim_x * dim_y)) / dim_x;

   float pos_x = units[0].get_position_x();
   float pos_y = units[0].get_position_y();
   float pos_z = units[0].get_position_z();

   float direction = 0.0f;

   units[0].set_speed (10.0f);

int condition = 0;

   if (x_block > floorf(pos_x - 0.5f)) {
      if (y_block > floorf(pos_y - 0.5f)) {
         direction = 0.785f;
condition = 1;
      }
      else if (y_block < floorf(pos_y + 0.5f)) {
         direction = 5.498f;
condition = 2;
      }
      else {
         direction = 0.0f;
condition = 3;
      }
   }
   else if (x_block < floorf(pos_x + 0.5f)) {
      if (y_block > floorf(pos_y - 0.5f)) {
         direction = 2.356f;
condition = 4;
      }
      else if (y_block < floorf(pos_y + 0.5f)) {
         direction = 3.927f;
condition = 5;
      }
      else {
         direction = 3.142f;
condition = 6;
      }
   }
   else {
      if (y_block > floorf(pos_y - 0.5f)) {
         direction = 1.571f;
condition = 7;
      }
      else if (y_block < floorf(pos_y + 0.5f)) {
         direction = 4.712f;
condition = 8;
      }
      else {
         units[0].set_speed (0.0f);
condition = 9;
      }
   }

std::cout << "(" << pos_x << ", " << pos_y << ")" << " (" << x_block << ", " << y_block << ")" << std::endl;
std::cout << "direction = " << direction << " (" << direction * 180.0f / 3.14159f << ") under condition " << condition << std::endl;
std::cout << std::endl;

//   direction = 0.785f;

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
