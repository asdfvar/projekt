#include "society.h"
#include <iostream>
#include <cstdlib>

Society::Society (void)
{
   dim_x = 40;
   dim_y = 40;
   dim_z = 40;

   map = new float[dim_x * dim_y * dim_z];

   for (int ind_z = 0, ind = 0; ind_z < dim_z; ind_z++)
   for (int ind_y = 0; ind_y < dim_y; ind_y++)
   for (int ind_x = 0; ind_x < dim_x; ind_x++, ind++)
   {
      if (rand() & 3 && ind_z == 20) map[ind] = 1.0f;
      else map[ind] = -1.0f;
   }
}

Society::~Society (void)
{
   delete[] map;
}

void Society::input (Control *control)
{
   control_queue.push (control);
}

void Society::update (float time_step)
{

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

      control_queue.pop();
   }

}

const float *Society::access_map (int *dim_x_out, int *dim_y_out, int *dim_z_out)
{
   *dim_x_out = dim_x;
   *dim_y_out = dim_y;
   *dim_z_out = dim_z;
   return static_cast<const float*>(map);
}
