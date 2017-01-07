#include "user.h"
#include "linalg.h"
#include "fileio.h"
#include <cmath>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

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

   forward_speed = 0.0f;
   right_speed   = 0.0f;
   left_speed    = 0.0f;
   up_speed      = 0.0f;
   down_speed    = 0.0f;

   /*
   ** User view-window attributes
   */
   window_distance = 1.0f;
   window_width    = 1.0f;
   window_height   = 1.0f;

   int data[8] = {3, 1, 4, 1, 5, 9, 2, 6};
   fio::write( "test.bin", 0, (char*)data, 4*8);
   fio::write( "test.bin", 8, (char*)data, 4*8);
   fio::read(  "test.bin" );
#if 0
   int numbers[20];
   fio::read ( "test.bin", 8, (void*)numbers, 4*8);
   for (int k = 0; k < 8; k++)
      std::cout << numbers[k];
   std::cout << std::endl;
#endif

}

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

void User::update( float dt)
{
   position[0] += direction[0] * forward_speed * dt;
   position[1] += direction[1] * forward_speed * dt;
   position[2] += direction[2] * forward_speed * dt;

   float right_direction[3];
   right_direction[0] =  direction[1];
   right_direction[1] = -direction[0];
   right_direction[2] =  direction[2];
   position[0] += right_direction[0] * right_speed * dt;
   position[1] += right_direction[1] * right_speed * dt;

   float left_direction[3];
   left_direction[0] = -direction[1];
   left_direction[1] =  direction[0];
   left_direction[2] =  direction[2];
   position[0] += left_direction[0] * left_speed * dt;
   position[1] += left_direction[1] * left_speed * dt;

   position[2] += up_speed * dt;

   position[2] += -down_speed * dt;
}

void User::move_upward( void)
{
   up_speed = 10.0f;
}

void User::move_downward( void)
{
   down_speed = 10.0f;
}

void User::move_right( void)
{
   right_speed = 10.0f;
}

void User::move_left( void)
{
   left_speed = 10.0f;
}

void User::move_forward( void)
{
   forward_speed = 10.0f;
}

void User::move_backward( void)
{
   forward_speed = -10.0f;
}

void User::stop_forward( void)
{
   forward_speed = 0.0f;
}

void User::stop_right( void)
{
   right_speed = 0.0f;
}

void User::stop_left( void)
{
   left_speed = 0.0f;
}

void User::stop_up( void)
{
   up_speed = 0.0f;
}

void User::stop_down( void)
{
   down_speed = 0.0f;
}
