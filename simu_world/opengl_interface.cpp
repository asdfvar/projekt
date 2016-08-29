#include "opengl_interface.h"

Opengl_interface::Opengl_interface(void)
{
   time_manager = new Time_manager( 1.0 / 60.0);
   int map_dim[3]   = {11, 11, 11};
   float map_pos[3] = {0.0f, 0.0f, 0.0f};
   map = new Map( 0, map_dim, map_pos);
}

void Opengl_interface::keyboardDown( const char key)
{

   switch (key) {
      case 'u':
         user.move_upward();
   }

}

Opengl_interface::~Opengl_interface(void)
{
   delete time_manager;
   delete map;
}
