#include "opengl_interface.h"

Opengl_interface::Opengl_interface(void)
{
   time_manager = new Time_manager( 1.0 / 60.0);
}

Opengl_interface::~Opengl_interface(void)
{
   delete time_manager;
}
