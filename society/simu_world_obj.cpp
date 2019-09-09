#include <iostream>
#include <sstream>
#include "society.h"
#include "change_direction.h"
#include "opengl_interface.h"
#include "hud.h"
#include "diagnostics.h"

/*
** function name: Society from: Society
*/
Society::Society ()
{
}

/*
** function name: keyboardDown from: Society
** glutGetModifiers() is true if the shift key is depressed
*/
void Society::keyboardDown( const char key )
{
}

/*
** function name: specialFunc from: Society
*/
void Society::specialFunc(int key, int x, int y)
{
}

/*
** function name: keyboardUp from: Society
*/
void Society::keyboardUp( const char key)
{
}

/*
** function name: mousePassive from Society
**
** (0,0) is defined in the top left corner of the window
** x is the column number of cells from left
** y is the row number of cells from top
*/
void Society::mousePassive( int x, int y )
{
}

/*
** destructor name: ~Society
*/
Society::~Society( void )
{
}

/*
** function name: idle from: Society
*/
void Society::idle( void )
{
   glutPostRedisplay ();
}

/*
** function name: display from: Society
*/
void Society::display( void )
{
   // clear this openGL buffer
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // swap this buffer for the old one
   glutSwapBuffers();
}
