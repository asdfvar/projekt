#ifndef OPENGL_INTERFACE_H
#define OPENGL_INTERFACE_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "time_manager.h"
#include "user.h"

class Opengl_interface
{

   public:

      Opengl_interface(void);
      ~Opengl_interface(void);

      /*
      ** Display to screen
      */
      void display(void);

      /*
      ** Main function kernel
      */
      void idle(void);

      /*
      ** Mouse motion input
      */
      void mouse_motion(void);

      /*
      ** Mouse click input
      */
      void mouse_click(void);

      /*
      ** Keyboard up-keys input
      */
      void keyboardUp( const char key);

      /*
      ** Keyboard down-keys input
      */
      void keyboardDown( const char key);

      /*
      ** Passive mouse location
      */
      void mousePassive( int x, int y);

   private:

      bool first_frame;

      int  mousePassivePosition[2];
      bool idle_done_mouse_passive;
      bool mouse_passive_done;

      Time_manager *time_manager;
      Map          *map;
      User          user;

float rcube;

};

#endif
