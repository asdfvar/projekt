#include "shape.h"
#include "sphere.h"

/*
 * Constructor NAME: sphere
 */

Sphere::Sphere ( float center_in[3],
                 float radius_in )
               : Shape (center_in)
{
   radius = radius_in;
}

bool Sphere::intersect (Ray    incomming_ray,
                        Ray   *new_ray,
                        float *distance)
{

   float p[3] = {incomming_ray.get_position(0),
                 incomming_ray.get_position(1),
                 incomming_ray.get_position(2)};

   float u[3] = {incomming_ray.get_direction(0),
                 incomming_ray.get_direction(1),
                 incomming_ray.get_direction(2)};

   bool intersection = geometry::intersect_sphere (
                                  p,
                                  u,
                                  center,
                                  radius);

   return intersection;

}
