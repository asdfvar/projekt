#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "simu_world.h"
#include "opengl_interface.h"

/*
** Define the interface with openGL
*/
Opengl_interface opengl_interface;

/*********************
 * Display to screen *
 *********************/

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);

  opengl_interface.display();

  glFlush();

}

/*******************
 * Idle processing *
 *******************/

void idle(void) {

   opengl_interface.idle();

   glutPostRedisplay();

}

/***************
 * Mouse click *
 ***************/

void mouse(
        int button,
        int state,
        int x,
        int y)
{

/*
   button
      GLUT_LEFT_BUTTON
      GLUT_MIDDLE_BUTTON
      GLUT_RIGHT_BUTTON
   state
      GLUT_UP
      GLUT_DOWN
*/

//opengl_interface.mouse(button, state, x, y);

}

void mouseMotion(int x, int y) {

   //opengl_interface.mouseMotion(x, y);

}


/*
** GLUT_WINDOW_X        X location in pixels (relative to the screen origin) of the current window
** GLUT_WINDOW_Y        Y location in pixels (relative to the screen origin) of the current window
** GLUT_WINDOW_WIDTH    Width in pixels of the current window
** GLUT_WINDOW_HEIGHT   Height in pixels of the current window
*/
void mousePassive(int x, int y)
{

   opengl_interface.mousePassive( x, y);
   glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {

   opengl_interface.keyboardUp( key);
}

void keyboardDown(unsigned char key, int x, int y)
{

   switch (key)
   {
      case 'q':
         std::cout << "program exit" << std::endl;
         exit(1);
         break;
      case 27: // escape key
         std::cout << "program exit" << std::endl;
         exit(1);
         break;
   }

   opengl_interface.keyboardDown( key);
}


/********
 * Main *
 ********/

void simu_world( int argc, char** argv )
{

   int windowsizex = 800;
   int windowsizey = 600;

   glutInit( &argc, argv);
   glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
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

}
