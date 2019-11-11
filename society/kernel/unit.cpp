#include <iostream>
#include "unit.h"
#include <cmath>

Unit::Unit (float position_x_in, float position_y_in, float position_z_in) :
   position_x (position_x_in), position_y (position_y_in), position_z (position_z_in)
{
   speed = 2.0f;
}

void Unit::move (float time_step, float direction)
{
static int count = 0;

std::cout << count++ << ":moving at " << speed << " in direction " << direction * 180.0f / 2.13145f << std::endl;
   position_x += speed * cosf (direction) * time_step;
   position_y += speed * sinf (direction) * time_step;
}

void Unit::set_speed (float speed_in)
{
   speed = speed_in;
}