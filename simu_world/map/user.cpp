#include <cmath>
#include "user.h"
#include "linalg.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

void User::get_direction( float *direction_out)
{

   direction_out[0] = direction[0];
   direction_out[1] = direction[1];
   direction_out[2] = direction[2];

   return;
}

void User::set_direction( float *direction_in)
{

   direction[0] = direction_in[0];
   direction[1] = direction_in[1];
   direction[2] = direction_in[2];

   return;
}

void User::get_position( float *position_out)
{

   position_out[0] = position[0];
   position_out[1] = position[1];
   position_out[2] = position[2];

   return;
}

float User::get_window_distance( void)
{
   return window_distance;
}

float User::get_window_width( void)
{
   return window_width;
}

User::User( void)
{
   /*
   ** User positional attributes
   */
   position[0] = 0.0f;
   position[1] = 0.0f;
   position[2] = 0.0f;

   direction[0] = 1.0f;
   direction[1] = 0.0f;
   direction[2] = 0.0f;

   speed = 0.0f;

   /*
   ** User view-window attributes
   */
   window_distance = 1.0f;
   window_width    = 1.0f;
   window_height   = 1.0f;

}

void User::update( float dt)
{
   position[0] += direction[0] * speed * dt;
   position[1] += direction[1] * speed * dt;
   position[2] += direction[2] * speed * dt;
}

void User::move_upward( void)
{
   position[2] += 0.5f;
}

void User::move_downward( void)
{
   position[2] -= 0.5f;
}

void User::move_right( void)
{
   float right_direction[3];
   right_direction[0] =  direction[1];
   right_direction[1] = -direction[0];
   right_direction[2] =  direction[2];
   position[0] += 0.5f * right_direction[0];
   position[1] += 0.5f * right_direction[1];
   position[2] += 0.5f * right_direction[2];
}

void User::move_left( void)
{
   float left_direction[3];
   left_direction[0] = -direction[1];
   left_direction[1] =  direction[0];
   left_direction[2] =  direction[2];
   position[0] += 0.5f * left_direction[0];
   position[1] += 0.5f * left_direction[1];
   position[2] += 0.5f * left_direction[2];

}

void User::move_forward( void)
{
   speed = 4.0f;
}

void User::move_backward( void)
{
   speed = -4.0f;
}

void User::stop( void)
{
   speed = 0.0f;
}
