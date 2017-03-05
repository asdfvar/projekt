#include <iostream>
#include <sstream>
#include "simu_world_obj.h"
#include "change_direction.h"
#include "opengl_interface.h"
#include "hud.h"
#include "text.h"

/*
** function name: Simu_world_obj from: Simu_world_obj
*/
Simu_world_obj::Simu_world_obj( pthread_barrier_t* IO_barrier_in )
{

   IO_barrier = IO_barrier_in;

   map = new Map( IO_barrier_in );

//   pthread_barrier_wait( IO_barrier_in );
   program_done = false;

   first_frame      = true;
   time_manager     = new Time_manager( 1.0 / 120.0 );
   semaphore        = new Semaphore(4);
   mode             = 1;

   std::ostringstream ss;
   ss << 42;
   std::string the_word = "the answer to the universe is ";
   the_word += ss.str();
   std::cout << the_word << std::endl;

   float color[3] = { 0.2f, 1.0f, 0.2f };
   map->set_phys_chunk_color( 6, 6, 6, color );
   color[0] = 1.0f; color[1] = 0.2f; color[2] = 0.2f;
   map->set_phys_chunk_color( 6, 7, 6, color );
   color[0] = 0.2f; color[1] = 0.2f; color[2] = 1.0f;
   map->set_phys_chunk_color( 6, 5, 6, color );
}

/*
** function name: keyboardDown from: Simu_world_obj
** glutGetModifiers() is true if the shift key is depressed
*/
void Simu_world_obj::keyboardDown( const char key )
{
   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   switch (key)
   {
      case 'q':
         std::cout << "program exit" << std::endl;
         program_done = true;
         // wait for IO threads to finish
         pthread_barrier_wait( IO_barrier );
         exit(1);
         break;
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
         user.move_upward();
         break;
      case 'c':
         user.move_downward();
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

/*
** function name: keyboardUp from: Simu_world_obj
*/
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
      case 32: // space bar
         user.stop_up();
         break;
      case 'c':
         user.stop_down();
         break;
   }
}

/*
** function name: mousePassive from Simu_world_obj
*/
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

/*
** destructor name: ~Simu_world_obj
*/
Simu_world_obj::~Simu_world_obj(void)
{
   delete map;
   delete time_manager;
   delete semaphore;
}

/*
** function name: idle from: Simu_world_obj
*/
void Simu_world_obj::idle( void)
{

   text.clear();

#ifdef DEBUG
   map->debug_info();
   user.debug_info();
   std::cout << std::endl;
#endif

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
         map->map_shift( user_position );

         text.populate("user position: ");
         text.populate( user_position[0] );
         text.populate( ", " );
         text.populate( user_position[1] );
         text.populate( ", " );
         text.populate( user_position[2] );
         text.new_line();

         int i_user_position[3] = { (int)floorf(user_position[0]),
                                    (int)floorf(user_position[1]),
                                    (int)floorf(user_position[2]) };

         map->diagnostics( i_user_position, &text);

      }

      /*
      ** Tell the passive-mouse task to enable
      */
      semaphore->decrement_task(1);
   }

   // Tell the display that text is done editing the text buffer
   text.done();

   /*
   ** Tell openGL to re-display
   */
   ogl::redisplay();
}

/*
** function name: Simu_world_obj
*/
void Simu_world_obj::display(void)
{
   // clear this openGL buffer
   ogl::opengl_clear();

   map->render_chunk( &user );

   hud::display();

   text.display_contents( -1.0f, 1.0f, 1.0f);

   // swap this buffer for the old one
   ogl::swap_buffers();
}

/*
** function name: simu_world_obj
*/
void Simu_world_obj::update_map()
{
   map->update();
}
