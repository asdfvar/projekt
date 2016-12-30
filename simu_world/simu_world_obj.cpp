#include "simu_world_obj.h"
#include "draw_scene.h"
#include "change_direction.h"
#include "opengl_interface.h"

Simu_world_obj::Simu_world_obj(void)
{
   first_frame      = true;
   time_manager     = new Time_manager( 1.0 / 120.0 );
   int chunk_dim[3]   = { 11, 11, 11 };
   float chunk_pos[3] = { 0.0f, 0.0f, 0.0f };
   semaphore        = new Semaphore(4);
   mode             = 1;

}

void Simu_world_obj::keyboardDown( const char key)
{

   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   switch (key)
   {
      case 13: // enter key
         mode = 1;
         glutWarpPointer( window_center_x, window_center_y);
         glutSetCursor(GLUT_CURSOR_NONE);
         break;
      case 27: // escape key
         mode = 0;
         glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
         break;
      case 32: // space bar
         if (glutGetModifiers() == 0)
         {
            user.move_upward();
         }
         else if (glutGetModifiers() == 1)
         {
            user.move_downward();
         }
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

void Simu_world_obj::keyboardUp( const char key)
{

   switch (key)
   {
      case 'w':
         user.stop_forward();
         break;
      case 'a':
         user.stop_left();
         break;
      case 's':
         user.stop_forward();
         break;
      case 'd':
         user.stop_right();
         break;
      case 32:
         user.stop_up();
         user.stop_down();
   }

}

void Simu_world_obj::mousePassive( int x, int y)
{

   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   if ( semaphore->task_pool(1) == 0 )
   {
      semaphore->increment_task(1);

      mousePassivePosition[0] = x;
      mousePassivePosition[1] = y;

      semaphore->decrement_task(0);
   }

}

Simu_world_obj::~Simu_world_obj(void)
{
   delete time_manager;
   delete semaphore;
}

void Simu_world_obj::idle( void)
{

   /*
   ** wait for the prescribed time to pass
   */
   time_manager->wait_for_time();

   user.update( time_manager->get_time_step() );

   int window_center_x  = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y  = glutGet(GLUT_WINDOW_HEIGHT) / 2;
   int window_center[2] = { glutGet(GLUT_WINDOW_WIDTH)  / 2,
                            glutGet(GLUT_WINDOW_HEIGHT) / 2 };

   if (first_frame)
   {

      ogl::opengl_initial_settings();

      glutWarpPointer( window_center[0], window_center[1]);
      mousePassivePosition[0] = window_center[0];
      mousePassivePosition[1] = window_center[1];
      first_frame             = false;
   }
   if( semaphore->task_pool(0) == 0 )
   {
      semaphore->increment_task(0);
      if (mode == 0)
      {
         glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
      }
      if (mode == 1)
      {

         glutSetCursor(GLUT_CURSOR_NONE);

         /*
         ** Move the direction of the user based from mouse motion
         */
         float direction[3];
         user.get_direction( direction );

         change_direction( direction,
                           window_center,
                           mousePassivePosition);
   
         user.set_direction( direction);
      
         glutWarpPointer( window_center_x, window_center_y);

         /*
         ** Update the chunk grid if the user position has exceeded the threshold
         */
         float user_position[3];

         user.get_position( user_position);
         map.update( user_position);

      }

      /*
      ** Tell the passive-mouse task to enable
      */
      semaphore->decrement_task(1);
   }

   /*
   ** Tell openGL to re-display
   */
   ogl::redisplay();
}

void Simu_world_obj::display(void)
{

   ogl::opengl_clear();

   draw_scene( &user,
               &map);

   ogl::swap_buffers();

}
