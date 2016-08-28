#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "simu_world.h"
#include "opengl_interface.h"

Opengl_interface opengl_interface;

/*********************
 * Display to screen *
 *********************/

void display(void) {

opengl_interface.display();

}

/*******************
 * Idle processing *
 *******************/

void idle(void) {

opengl_interface.idle();

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


void mousePassive(int x, int y) {

   std::cout << "x = " << x << std::endl;
   std::cout << "y = " << y << std::endl;
}

void keyboardUp(unsigned char key, int x, int y) {

   std::cout << key << std::endl;
}

void keyboardDown(unsigned char key, int x, int y)
{
switch (key) {
   case 'q':
      std::cout << "program exit" << std::endl;
      exit(1);
      break;
}
}


/********
 * Main *
 ********/

void simu_world( int argc, char** argv )
{

   int windowsizex = 800;
   int windowsizey = 600;

   glutInit( &argc, argv);
   glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE);
   glutInitWindowSize( windowsizex, windowsizey);
   glutInitWindowPosition( 50, 50);
   glutCreateWindow( "Simu World");

   glutDisplayFunc( display);
   glutMouseFunc( mouse);
   glutMotionFunc( mouseMotion);
   glutPassiveMotionFunc( mousePassive);
   glutKeyboardUpFunc( keyboardUp);
   glutKeyboardFunc( keyboardDown);
   glutIdleFunc( idle);

   glutMainLoop();

}
