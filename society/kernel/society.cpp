#include "society.h"
#include <iostream>

Society::Society (void)
{
   map = new float[40 * 40 * 40];
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
