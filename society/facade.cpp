#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include "facade.h"
#include "control.h"
#include "graphics.h"

/*
** function name: Facade from: Facade
*/
Facade::Facade (void)
{
   gettimeofday (&start, NULL);

   unit_positions_x = new float[100];
   unit_positions_y = new float[100];
   unit_positions_z = new float[100];

   transform[0] = 1.0f; transform[1] = 0.0f;
   transform[2] = 0.0f; transform[3] = 1.0f;

   translation[0] = 0.0f;
   translation[1] = 0.0f;
}

/*
** function name: keyboardUp from: Facade
*/
void Facade::keyboardUp (const char key, int x, int y)
{
   control = new KeyboardUp (key, x, y);
   society.input (control);
}

/*
** function name: keyboardDown from: Facade
** glutGetModifiers() is true if the shift key is depressed
*/
void Facade::keyboardDown (const char key, int x, int y)
{
   control = new KeyboardDown (key, x, y);
   society.input (control);
}

/*
** function name: specialFunc from: Facade
*/
void Facade::specialFunc (int key, int x, int y)
{
   control = new KeyboardSpecial (key, x, y);
   society.input (control);
}

void Facade::mouseClick (int button, int state, int x, int y)
{
   control = new MouseClick (button, state, x, y);
   society.input (control);
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
}

void Facade::mouseMotion (int x, int y)
{
}

/*
** destructor name: ~Facade
*/
Facade::~Facade (void)
{
   delete[] unit_positions_x;
   delete[] unit_positions_y;
   delete[] unit_positions_z;
}

/*
** function name: idle from: Facade
*/
void Facade::idle (void) {
   double time_step = 0.01;

   double time_taken;

   do {
      gettimeofday (&end, NULL);
      time_taken = (end.tv_sec * 1000000 + end.tv_usec -
                   (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;
   } while (time_taken < time_step);

   society.update (time_step);
   gettimeofday (&start, NULL);

   glutPostRedisplay ();
}

/*
** function name: display from: Facade
*/
void Facade::display (void)
{
   // clear this openGL buffer
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   int map_layer = 20;

   int dim_x, dim_y, dim_z;
   const float *map = society.access_map (&dim_x, &dim_y, &dim_z);
   int map_dims[3] = { dim_x, dim_y, dim_z };

   int num_units = society.get_unit_positions (
         unit_positions_x,
         unit_positions_y,
         unit_positions_z);

   draw_units (
         transform,
         translation,
         unit_positions_x,
         unit_positions_y,
         unit_positions_z,
         map_dims,
         num_units,
         map_layer);

   draw_map (
         transform,
         translation,
         map,
         map_dims,
         map_layer);

   // swap this buffer for the old one
   glutSwapBuffers();
}
