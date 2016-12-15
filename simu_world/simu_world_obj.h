#ifndef SIMU_WORLD_H
#define SIMU_WORLD_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "time_manager.h"
#include "user.h"
#include "semaphore.h"
#include "opengl_interface.h"

class Simu_world_obj
{

   public:

      Simu_world_obj(void);
     ~Simu_world_obj(void);

      /*
      ** Display to screen
      */
      void display(void);

      /*
      ** Idle processing
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
      int  mode;

      Semaphore             *semaphore;
      Time_manager          *time_manager;
      ogl::Opengl_interface  opengl_interface;
      Map                   *map;
      User                   user;

};

#endif