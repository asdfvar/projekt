#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <sstream>
#include "facade.h"
#include "control.h"

/*
** function name: Facade from: Facade
*/
Facade::Facade (void)
{
}

/*
** function name: keyboardDown from: Facade
** glutGetModifiers() is true if the shift key is depressed
*/
void Facade::keyboardDown (const char key, int x, int y)
{
std::cout << "key down = " << key << " @ (" << x << ", " << y << ")" << std::endl;

   Keyboard_down control (key, x, y);
   Control_queue.push (control);
}

/*
** function name: specialFunc from: Facade
*/
void Facade::specialFunc (int key, int x, int y)
{
}

/*
** function name: keyboardUp from: Facade
*/
void Facade::keyboardUp (const char key, int x, int y)
{
}

void Facade::mouse ( int button, int state, int x, int y)
{
std::cout << "mouse click button " << button << " state " << state << " @ (" << x << ", " << y << ")" << std::endl;
}

/*
** function name: mousePassive from Facade
**
** (0,0) is defined in the top left corner of the window
** x is the column number of cells from left
** y is the row number of cells from top
*/
void Facade::mousePassive (int x, int y)
{
std::cout << "mouse passive " << " @ (" << x << ", " << y << ")" << std::endl;
}

void Facade::mouseMotion (int x, int y)
{
std::cout << "mouse motion " << " @ (" << x << ", " << y << ")" << std::endl;
}

/*
** destructor name: ~Facade
*/
Facade::~Facade (void)
{
}

/*
** function name: idle from: Facade
*/
void Facade::idle (void)
{
   glutPostRedisplay ();
}

/*
** function name: display from: Facade
*/
void Facade::display (void)
{
   // clear this openGL buffer
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // swap this buffer for the old one
   glutSwapBuffers();
}
