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

bool Sphere::intersect (Ray ray)
{

   float p[3] = {ray.get_position(0),
                 ray.get_position(1),
                 ray.get_position(2)};

   float u[3] = {ray.get_direction(0),
                 ray.get_direction(1),
                 ray.get_direction(2)};

   bool intersection = geometry::intersect_sphere (
                                  p,
                                  u,
                                  center,
                                  radius);

   return intersection;

}
