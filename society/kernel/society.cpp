#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "map.h"
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

   int dim[3] = {dim_x, dim_y, dim_z};

   Map = new MAP (dim, map_layer);

   bool found = false;

   float unit_x = 0.0f;
   float unit_y = 0.0f;
   float unit_z = 0.0f;

   for (int ind_z = 0, ind = 0; ind_z < dim_z && !found; ind_z++) {
      for (int ind_y = 0; ind_y < dim_y && !found; ind_y++) {
         for (int ind_x = 0; ind_x < dim_x && !found; ind_x++, ind++)
         {
            const float *map = Map->access_map ();
            if (map[ind] > 0.0f && ind_z == map_layer) {
               found = true;
               unit_x = (float)ind_x     + 0.5f;
               unit_y = (float)ind_y     + 0.5f;
               unit_z = (float)map_layer + 0.5f;
            }
         }
      }
   }

   float *scratch = new float[2 * dim_x * dim_y * dim_z];
   Unit *unit = new Unit (unit_x, unit_y, unit_z, Map, scratch);

   units.push_back (unit);

}

Society::~Society (void)
{
   delete Map;
}

void Society::input (Control *control)
{
   control_queue.push (control);
}

void Society::update (float time_step)
{
   if (!control_queue.empty())
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
            int destination[3];

            destination[0] = block_x;
            destination[1] = block_y;
            destination[2] = map_layer;

            units[0]->set_destination (destination);
            units[0]->set_speed (4.0f);
         }
      }

      control_queue.pop();
   }

   units[0]->update (time_step);
}

const float *Society::access_map (int *dim_x_out, int *dim_y_out, int *dim_z_out)
{
   *dim_x_out = dim_x;
   *dim_y_out = dim_y;
   *dim_z_out = dim_z;
   return static_cast<const float*>(Map->access_map ());
}

int Society::get_unit_positions (float *x, float *y, float *z)
{
   int ind = 0;
   for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++, ind++) {
      x[ind] = (*it)->get_position_x();
      y[ind] = (*it)->get_position_y();
      z[ind] = (*it)->get_position_z();
   }

   return ind;
}
