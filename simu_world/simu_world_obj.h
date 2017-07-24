#ifndef SIMU_WORLD_H
#define SIMU_WORLD_H

#include <pthread.h>
#include <stdlib.h>
#include "time_manager.h"
#include "user.h"
#include "map.h"
#include "semaphore.h"
#include "menu.h"
#include "opengl_interface.h"
#include "text.h"

class Simu_world_obj
{

   public:

      Simu_world_obj( pthread_barrier_t* IO_barrier_in,
                      pthread_barrier_t* update_barrier_in );

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

      void update_map( void );

      bool program_done;

      pthread_barrier_t* IO_barrier;
      pthread_barrier_t* update_barrier;

   private:

      bool first_frame;

      int  mousePassivePosition[2];
      int  mode;

      Semaphore*             semaphore;
      Menu*                  current_menu;
      Time_manager*          time_manager;
      Map*                   map;
      User                   user;
      Text                   text;

};

#endif
