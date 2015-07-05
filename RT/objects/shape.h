#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"

/*
 * Class NAME: Shape
 */

class Shape
{

   public:

      Shape( float center[3] );
      ~Shape (void);

      float center[3];

};

#endif
