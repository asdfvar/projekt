#include "opengl_interface.h"

void Opengl_interface::idle( void)
{
   /*
   ** wait for the prescribed time to pass
   */
   time_manager->wait_for_time();

}
