#ifndef OPENGL_INTERFACE_H
#define OPENGL_INTERFACE_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

namespace ogl
{
  class Opengl_interface
  {
     public:
        Opengl_interface( void );


     private:
  };

  void Clear ( GLbitfield mask  );
  int  Get   ( GLenum state     );

}

#endif
