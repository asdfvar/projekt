#include <iostream>
#include <sstream>
#include "simu_world_obj.h"
#include "change_direction.h"
#include "opengl_interface.h"
#include "hud.h"
#include "diagnostics.h"

/*
** function name: Simu_world_obj from: Simu_world_obj
*/
Simu_world_obj::Simu_world_obj( pthread_barrier_t* IO_barrier_in,
                                pthread_barrier_t* update_barrier_in )
{
   IO_barrier     = IO_barrier_in;
   update_barrier = update_barrier_in;

   map = new Map( IO_barrier,
                  update_barrier );

   program_done = false;

   first_frame      = true;
   time_manager     = new Time_manager( 1.0 / 60.0 );
   semaphore        = new Semaphore(4);
   mode             = 1;

   std::ostringstream ss;
   ss << 42;
   std::string the_word = "the answer to the universe is ";
   the_word += ss.str();
   std::cout << the_word << std::endl;

#if 0
   float color[3] = { 0.2f, 1.0f, 0.2f };
   map->set_phys_chunk_color( 6, 6, 6, color );
   color[0] = 1.0f; color[1] = 0.2f; color[2] = 0.2f;
   map->set_phys_chunk_color( 6, 7, 6, color );
   color[0] = 0.2f; color[1] = 0.2f; color[2] = 1.0f;
   map->set_phys_chunk_color( 6, 5, 6, color );
#endif
}

/*
** function name: keyboardDown from: Simu_world_obj
** glutGetModifiers() is true if the shift key is depressed
*/
void Simu_world_obj::keyboardDown( const char key )
{
   int window_center_x = ogl::get_window_width() / 2;
   int window_center_y = ogl::get_window_height() / 2;

   switch (key)
   {
      case 'q':
         std::cout << "program exit" << std::endl;
         program_done = true;
         // wait for IO threads to finish
         pthread_barrier_wait( update_barrier );
         pthread_barrier_wait( IO_barrier );
         exit(1);
         break;
      case 13: // enter key
         mode = 1;
         ogl::warp_pointer( window_center_x, window_center_y );
         ogl::set_cursor_none();
         break;
      case 27: // escape key
         mode = 0;
         ogl::set_cursor_arrow();
         break;
      case 32: // space bar
         user.move_upward();
         break;
      case 'x':
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
      case 'x':
         user.stop_down();
         break;
   }
}

/*
** function name: mousePassive from Simu_world_obj
**
** (0,0) is defined in the top left corner of the window
** x is the column number of cells from left
** y is the row number of cells from top
*/
void Simu_world_obj::mousePassive( int x, int y )
{

   int window_center_x = ogl::get_window_width() / 2;
   int window_center_y = ogl::get_window_height() / 2;

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
Simu_world_obj::~Simu_world_obj( void )
{
   delete map;
   delete time_manager;
   delete semaphore;
}

/*
** function name: idle from: Simu_world_obj
*/
void Simu_world_obj::idle( void )
{

#ifdef DEBUG
   map->debug_info();
   user.debug_info();
   std::cout << std::endl;
#endif

   /*
   ** wait for the prescribed time to pass
   */
   time_manager->wait_for_time();

   user.update( time_manager->get_time_step_actual_factor() );

   int window_width     = ogl::get_window_width();
   int window_height    = ogl::get_window_height();
   int window_center_x  = window_width  / 2;
   int window_center_y  = window_height / 2;

   if ( first_frame )
   {
      ogl::opengl_initial_settings();

      ogl::warp_pointer( window_center_x, window_center_y );
      mousePassivePosition[0] = window_center_x;
      mousePassivePosition[1] = window_center_y;
      first_frame             = false;
   }
   if( semaphore->task_pool(0) == 0 )
   {
      semaphore->increment_task(0);
      if (mode == 0)
      {
         ogl::set_cursor_arrow();
      }
      if (mode == 1)
      {

         g_text.clear();

         ogl::set_cursor_none();

         /*
         ** Move the direction of the user based from mouse motion
         */
         float direction[3];
         user.get_direction( direction );

         /*
         ** define x/y offsets in physical units invariant
         ** to the window cell dimensions with +x in the right
         ** directions and +y in the up direction
         */
         float x_offset = (float)( mousePassivePosition[0] - window_center_x ) *
                          user.get_window_width() / (float)window_width;
         float y_offset = (float)( window_center_y - mousePassivePosition[1] ) *
                          user.get_window_height() / (float)window_height;

         change_direction( direction,
                           user.get_window_distance(),
                           x_offset,
                           y_offset,
                           3.5f );
   
         user.set_direction( direction );
      
         ogl::warp_pointer( window_center_x, window_center_y );

         /*
         ** Update the chunk grid if the user position has exceeded the threshold
         */
         float user_position[3];

         user.get_position( user_position );
         map->map_shift( user_position );

         g_text.populate("user position: ");
         g_text.populate( user_position[0] );
         g_text.populate( ", " );
         g_text.populate( user_position[1] );
         g_text.populate( ", " );
         g_text.populate( user_position[2] );
         g_text.new_line();
         g_text.populate("frame time: ");
         g_text.populate( time_manager->get_time_step_actual() );
         g_text.populate(", ");
         g_text.populate( 1.0f / time_manager->get_time_step_actual() );
         g_text.populate(" Hz");
         g_text.new_line();

         int i_user_position[3] = { (int)floorf(user_position[0]),
                                    (int)floorf(user_position[1]),
                                    (int)floorf(user_position[2]) };

         map->diagnostics( i_user_position );

         g_text.done_editing();

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

/*
** function name: Simu_world_obj
*/
void Simu_world_obj::display( void )
{
   // clear this openGL buffer
   ogl::opengl_clear();

   float user_position[3];
   float user_direction[3];
   float window_distance = user.get_window_distance();
   float window_width    = user.get_window_width();

   user.get_position(  user_position );
   user.get_direction( user_direction );

   map->render_chunk( user_position,
                      user_direction,
                      window_distance,
                      window_width );

   hud::display();

   g_text.display_contents( -1.0f, 1.0f, 1.0f);

   // swap this buffer for the old one
   ogl::swap_buffers();
}

/*
** function name: update_map from:  simu_world_obj
*/
void Simu_world_obj::update_map()
{
   map->update();
}
