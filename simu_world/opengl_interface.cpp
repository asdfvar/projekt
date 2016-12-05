#include "opengl_interface.h"

Opengl_interface::Opengl_interface(void)
{
   first_frame             = true;
   time_manager            = new Time_manager( 1.0 / 60.0);
   int map_dim[3]          = {11, 11, 11};
   float map_pos[3]        = {0.0f, 0.0f, 0.0f};
   map                     = new Map( 0, map_dim, map_pos);
   rcube                   = 0.0f;
   idle_done_mouse_passive = true;
   mouse_passive_done      = true;
}

void Opengl_interface::keyboardDown( const char key)
{

   switch (key) {
      case 'u':
         user.move_upward();
         break;
      case 'n':
         user.move_downward();
         break;
      case 'w':
         user.move_forward();
         break;
      case 's':
         user.move_backward();
         break;
      case 'a':
         user.move_left();
         break;
      case 'd':
         user.move_right();
         break;
   }

}

void Opengl_interface::keyboardUp( const char key)
{

   switch (key) {
      case 'w':
         user.stop();
         break;
      case 's':
         user.stop();
         break;
   }

}

void Opengl_interface::mousePassive( int x, int y)
{

   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   if (!mouse_passive_done && (x != window_center_x || y != window_center_y))
   {
      mousePassivePosition[0] = x;
      mousePassivePosition[1] = y;
      mouse_passive_done      = true;
   }

}

Opengl_interface::~Opengl_interface(void)
{
   delete time_manager;
   delete map;
}
