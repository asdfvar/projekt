#include "opengl_interface.h"
#include <iostream>

ogl::Opengl_interface::Opengl_interface( void )
{
   // pass
}

void ogl::Clear( GLbitfield mask)
{
   glClear( mask );
}

int ogl::Get( GLenum state )
{
   return glutGet( state );
}
