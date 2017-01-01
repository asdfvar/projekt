#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <pthread.h>
#include "simu_world.h"
#include "simu_world_obj.h"
#include "io_manager.h"

/* Define the interface with openGL */
Simu_world_obj simu_world_obj;

typedef struct ARGS
{
   Simu_world_obj *simu_world_obj;
} ARGS;

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
   simu_world_obj.mousePassive( x, y);
}

/*****************
 ** Keyboard up **
 *****************/
void keyboardUp(unsigned char key, int x, int y) {

   simu_world_obj.keyboardUp( key);
}

/*******************
 ** Keyboard down **
 *******************/
void keyboardDown(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'q':
         std::cout << "program exit" << std::endl;
         exit(1);
         break;
   }

   simu_world_obj.keyboardDown( key);
}


/**********
 ** Main **
 **********/
void simu_world( int argc, char** argv )
{

   ARGS args;
   args.simu_world_obj = &simu_world_obj;
   pthread_t IO_thread;

   /*
   ** Create the IO manager thread
   */
   pthread_create( &IO_thread, NULL, io_manager, (void*)&args );

   int windowsizex = 800;
   int windowsizey = 800;

   glutInit( &argc, argv);
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize( windowsizex, windowsizey);
   glutInitWindowPosition( 50, 50);
   glutCreateWindow( "Simu World");

   glutDisplayFunc(       display);
   glutMouseFunc(         mouse);
   glutMotionFunc(        mouseMotion);
   glutPassiveMotionFunc( mousePassive);
   glutKeyboardUpFunc(    keyboardUp);
   glutKeyboardFunc(      keyboardDown);
   glutIdleFunc(          idle);

   glutMainLoop();

   /*
   ** wait for the IO manager thread to finish
   */
   pthread_join( IO_thread, NULL);

}
