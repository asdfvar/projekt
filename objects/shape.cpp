#include "shape.h"

/*
 * Constructor NAME: shape
 */

Shape::Shape (float center_in[3] )
{
   center[0] = center_in[0];
   center[1] = center_in[1];
   center[2] = center_in[2];
}

Shape::~Shape ( void ) { }
