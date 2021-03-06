#include "opengl_interface.h"
#include <pthread.h>
#include "simu_world.h"
#include "simu_world_obj.h"
#include "io_manager.h"
#include "thread_args.h"

/*
   There's a great thread barrier example here:
   https://github.com/angrave/SystemProgramming/wiki/Sample-program-using-pthread-barriers
*/

pthread_t         IO_thread;
pthread_barrier_t IO_barrier;
pthread_barrier_t update_barrier;
pthread_mutex_t   IO_mutex;

/* Define the interface with openGL */
Simu_world_obj simu_world_obj( &IO_barrier,
                               &update_barrier );

/***********************
 ** Display to screen **
 ***********************/
void display(void)
{
  simu_world_obj.display();
}

/*********************
 ** Idle processing **
 *********************/
void idle(void)
{
   simu_world_obj.idle();
}

/*****************
*** Mouse click **
******************
**   button
**      GLUT_LEFT_BUTTON
**      GLUT_MIDDLE_BUTTON
**      GLUT_RIGHT_BUTTON
**   state
**      GLUT_UP
**      GLUT_DOWN
*/
void mouse( int button,
            int state,
            int x,
            int y)
{
// simu_world_obj.mouse(button, state, x, y);
}

/**************************
 ** Active mouse control **
 **************************/
void mouseMotion(int x, int y)
{
   // simu_world_obj.mouseMotion(x, y);
}


/***************************
*** Passive mouse control **
****************************
** GLUT_WINDOW_X        X location in pixels (relative to the screen origin) of the current window
** GLUT_WINDOW_Y        Y location in pixels (relative to the screen origin) of the current window
** GLUT_WINDOW_WIDTH    Width in pixels of the current window
** GLUT_WINDOW_HEIGHT   Height in pixels of the current window
*/
void mousePassive(int x, int y)
{
   simu_world_obj.mousePassive( x, y );
}

/*****************
 ** Keyboard up **
 *****************/
void keyboardUp(unsigned char key, int x, int y)
{
   simu_world_obj.keyboardUp( key );
}

/*******************
 ** Keyboard down **
 *******************/
void keyboardDown(unsigned char key, int x, int y)
{
   simu_world_obj.keyboardDown( key );
}

void specialFunc(int key, int x, int y)
{
   simu_world_obj.specialFunc( key, x, y );
}


/**********
 ** Main **
 **********/
void simu_world( int argc, char** argv )
{

#ifdef __linux__
std::cout << "Linux" << std::endl;
#elif _WIN32
std::cout << "Windows" << std::endl;
#endif

   pthread_barrier_init( &IO_barrier, NULL, 2);
   pthread_barrier_init( &update_barrier, NULL, 2);
   pthread_mutex_init( &IO_mutex, NULL );

   ARGS args;

   args.simu_world_obj = &simu_world_obj;
   args.IO_barrier     = &IO_barrier;
   args.update_barrier = &update_barrier;
   args.IO_mutex       = &IO_mutex;

   /*
   ** Create the IO manager thread
   */
   pthread_create( &IO_thread, NULL, io_manager, (void*)&args );

   int windowsizex = 1600;
   int windowsizey = 1000;

   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutInitWindowSize( windowsizex, windowsizey );
   glutInitWindowPosition( 50, 50 );
   glutCreateWindow( "simu" );

   glutDisplayFunc(       display      );
   glutMouseFunc(         mouse        );
   glutMotionFunc(        mouseMotion  );
   glutPassiveMotionFunc( mousePassive );
   glutKeyboardUpFunc(    keyboardUp   );
   glutKeyboardFunc(      keyboardDown );
   glutSpecialFunc(       specialFunc  );
   glutIdleFunc(          idle         );

   glutMainLoop();

   /*
   ** wait for the IO manager thread to finish
   */
   pthread_join( IO_thread, NULL);
   pthread_mutex_destroy( &IO_mutex );

}
