#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

#include "facade.h"

/* Define the interface with openGL */
Facade facade;

/***********************
 ** Display to screen **
 ***********************/
void display(void)
{
  facade.display ();
}

/*********************
 ** Idle processing **
 *********************/
void idle(void)
{
   facade.idle ();
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
void mouseClick (
      int button,
      int state,
      int x,
      int y)
{
   facade.mouseClick (button, state, x, y);
}

/**************************
 ** Active mouse control **
 **************************/
void mouseMotion(int x, int y)
{
   facade.mouseMotion (x, y);
}


/***************************
*** Passive mouse control **
****************************
** GLUT_WINDOW_X        X location in pixels (relative to the screen origin)
**                      of the current window
** GLUT_WINDOW_Y        Y location in pixels (relative to the screen origin)
**                      of the current window
** GLUT_WINDOW_WIDTH    Width in pixels of the current window
** GLUT_WINDOW_HEIGHT   Height in pixels of the current window
*/
void mousePassive (int x, int y)
{
   facade.mousePassive (x, y);
}

/*****************
 ** Keyboard up **
 *****************/
void keyboardUp (unsigned char key, int x, int y)
{
   facade.keyboardUp (key, x, y);
}

/*******************
 ** Keyboard down **
 *******************/
void keyboardDown (unsigned char key, int x, int y)
{
   if (key == 'q') exit (1);

   facade.keyboardDown (key, x, y);
}

// Special key values
void specialFunc(int key, int x, int y)
{
   facade.specialFunc (key, x, y);
}

void specialUpFunc(int key, int x, int y)
{
   facade.specialUpFunc (key, x, y);
}

/**********
 ** Main **
 **********/
int main (int argc, char** argv)
{
   int windowsizex = 800;
   int windowsizey = 800;

   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (windowsizex, windowsizey);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("whee");

   glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);

   float light_ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
   float light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
   float light_specular[] = { 0.8, 0.8, 0.8, 1.0 };
   float light_position[] = { 0.0, 0.0, 1.0, 0.0 };

   glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient  );
   glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse  );
   glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);

   glEnable (GL_LIGHT0        );
   glEnable (GL_LESS          );
   glEnable (GL_DEPTH_TEST    );
   glEnable (GL_LIGHTING      );
   glEnable (GL_COLOR_MATERIAL);

   glDepthFunc (GL_LESS);

   glutDisplayFunc       (display      );
   glutMouseFunc         (mouseClick   );
   glutMotionFunc        (mouseMotion  );
   glutPassiveMotionFunc (mousePassive );
   glutKeyboardUpFunc    (keyboardUp   );
   glutKeyboardFunc      (keyboardDown );
   glutSpecialFunc       (specialFunc  );
   glutSpecialUpFunc     (specialUpFunc);
   glutIdleFunc          (idle         );

   glutMainLoop();

   return 0;
}
