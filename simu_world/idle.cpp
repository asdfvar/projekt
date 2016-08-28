#include "opengl_interface.h"
#include <iostream>

void Opengl_interface::idle( void)
{
   /*
   ** wait for the prescribed time to pass
   */
   time_manager->wait_for_time();

   user.update( time_manager->get_time_step() );

}
