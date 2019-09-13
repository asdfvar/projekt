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

   KeyboardDown *control = new KeyboardDown (key, x, y);

   control_lock.lock();
   Control_queue.push (control);
   control_lock.unlock();

   current_control = control;
}

/*
** function name: specialFunc from: Facade
*/
void Facade::specialFunc (int key, int x, int y)
{
   KeyboardSpecial *control = new KeyboardSpecial (key, x, y);
   
   control_lock.lock ();
   Control_queue.push (control);
   control_lock.unlock();

   current_control = control;
}

/*
** function name: keyboardUp from: Facade
*/
void Facade::keyboardUp (const char key, int x, int y)
{
   KeyboardUp *control = new KeyboardUp (key, x, y);
   
   control_lock.lock ();
   Control_queue.push (control);
   control_lock.unlock();

   current_control = control;
}

void Facade::mouseClick (int button, int state, int x, int y)
{
std::cout << "mouse click button " << button << " state " << state << " @ (" << x << ", " << y << ")" << std::endl;
   MouseClick *control = new MouseClick (button, state, x, y);
   
   control_lock.lock ();
   Control_queue.push (control);
   control_lock.unlock();

   current_control = control;
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
   MousePassive *control = new MousePassive (x, y);
   
   control_lock.lock ();
   Control_queue.push (control);
   control_lock.unlock();

   current_control = control;
}

void Facade::mouseMotion (int x, int y)
{
std::cout << "mouse motion " << " @ (" << x << ", " << y << ")" << std::endl;
   MouseMotion *control = new MouseMotion (x, y);
   
   control_lock.lock ();
   Control_queue.push (control);
   control_lock.unlock();

   current_control = control;
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

   control_lock.lock ();
   if (Control_queue.empty() == false) {
      Control_queue.pop ();
   }
   control_lock.unlock();

   Control *control = current_control;

   MousePassive *mp = dynamic_cast<MousePassive*>(control);
   if (mp != 0) {
      std::cout << "wqerasdfasdfasdfasdf!" << std::endl;
   }

   KeyboardDown *kd = dynamic_cast<KeyboardDown*>(control);
   if (kd != 0) {
      std::cout << "DOWN!" << std::endl;
   }

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
