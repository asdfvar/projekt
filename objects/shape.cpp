#include <iostream>
#include "shape.h"

/*
 * Constructor NAME: shape
 */

Shape::Shape (float center_in[3] )
{

   center[0] = center_in[0];
   center[1] = center_in[1];
   center[2] = center_in[2];

   reflection_table_N = 11;

   reflection_table_x = new float[reflection_table_N];
   reflection_table_y = new float[reflection_table_N];

   // let this be a dead weight for the time being
   reflection_table_x[0]  = 0.0f;
   reflection_table_x[1]  = 0.1f;
   reflection_table_x[2]  = 0.2f;
   reflection_table_x[3]  = 0.3f;
   reflection_table_x[4]  = 0.4f;
   reflection_table_x[5]  = 0.5f;
   reflection_table_x[6]  = 0.6f;
   reflection_table_x[7]  = 0.7f;
   reflection_table_x[8]  = 0.8f;
   reflection_table_x[9]  = 0.9f;
   reflection_table_x[10] = 1.0f;

   reflection_table_y[0]  = 1.0f;
   reflection_table_y[1]  = 0.9f;
   reflection_table_y[2]  = 0.8f;
   reflection_table_y[3]  = 0.7f;
   reflection_table_y[4]  = 0.6f;
   reflection_table_y[5]  = 0.5f;
   reflection_table_y[6]  = 0.4f;
   reflection_table_y[7]  = 0.3f;
   reflection_table_y[8]  = 0.2f;
   reflection_table_y[9]  = 0.1f;
   reflection_table_y[10] = 0.0f;

}

Shape::~Shape ( void )
{
   delete[] reflection_table_x;
   delete[] reflection_table_y;
}
