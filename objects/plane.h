#ifndef PLANE_H
#define PLANE_H

#include "ray.h"

class Plane : public Shape
{

   public:

      Plane( float p1[3],
             float p2[3],
             float p3[3],
             float color[3],
             float reflectivity);

      bool intersect(
                 Ray    incoming_ray,
                 Ray   *new_ray,
                 float *normal,
                 float *distance,
                 float color_intensities[],
                 float *reflectivity_out,
                 float **reflection_table_x_in,
                 float **reflection_table_y_in,
                 int   *reflection_table_N_in);

   private:

      float p1[3], p2[3], p3[3];

};

#endif
