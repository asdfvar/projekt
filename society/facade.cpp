#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include "facade.h"
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

   map_layer = 20;
}

/*
** function name: keyboardUp from: Facade
*/
void Facade::keyboardUp (const char key, int x, int y)
{
}

/*
** function name: keyboardDown from: Facade
** glutGetModifiers() is true if the shift key is depressed
*/
void Facade::keyboardDown (const char key, int x, int y)
{
}

/*
** function name: specialFunc from: Facade
*/
void Facade::specialFunc (int key, int x, int y)
{
}

void Facade::mouseClick (int button, int state, int x, int y)
{
   std::cout << "MOUSE CLICK " << state << std::endl;

   int window_width  = glutGet (GLUT_WINDOW_WIDTH);
   int window_height = glutGet (GLUT_WINDOW_HEIGHT);

   if (button == 0 && state == 0)
   {
      float window[2];
      window[0] = 2.0f * (float)x / (float)window_width - 1.0f;
      window[1] = 1.0f - 2.0f * (float)y / (float)window_height;

      const float det = transform[0] * transform[3] - transform[1] * transform[2];
      const float invDet = 1.0f / det;

      const float inv_transform[4] = { invDet * transform[3], -invDet * transform[1],
         -invDet * transform[2],  invDet * transform[0] };

      window[0] -= translation[0];
      window[1] -= translation[1];

      float temp = window[0];
      window[0] = window[0] * inv_transform[0] +
         window[1] * inv_transform[1];
      window[1] = temp      * inv_transform[2] +
         window[1] * inv_transform[3];

      int dim_x, dim_y, dim_z;
      society.access_map (&dim_x, &dim_y, &dim_z);

      float fcell[2];
      fcell[0] = (window[0] + 1.0f) / 2.0f * (float)dim_x;
      fcell[1] = (window[1] + 1.0f) / 2.0f * (float)dim_y;

      int cell[2];
      cell[0] = (int)fcell[0];
      cell[1] = (int)fcell[1];

      int destination[3];

      destination[0] = cell[0];
      destination[1] = cell[1];
      destination[2] = map_layer;

      society.set_destination (destination);
   }
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
void Facade::idle (void)
{
   double time_step = 0.01;

   double time_taken;

   do {
      gettimeofday (&end, NULL);
      time_taken = (end.tv_sec * 1000000 + end.tv_usec -
                   (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;
   } while (time_taken < time_step);

   // Update society at this time step
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
