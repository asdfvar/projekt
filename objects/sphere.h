#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "geometry.h"

/*
 * Constructor NAME: sphere
 */

class Sphere : public Shape
{

   public:

      Sphere( float center[3],
              float radius,
              float color[3],
              float reflectivity);

      float radius;

      bool intersect( Ray     incomming_ray,
                      Ray    *new_ray,
                      float  *normal,
                      float  *distance,
                      float   color_intensities[],
                      float  *reflectivity,
                      float **reflection_table_x,
                      float **reflection_table_y,
                      int    *reflection_table_N);

};

#endif
