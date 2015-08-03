#include "linalg.h"
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

   float position[3] = {incomming_ray.get_position(0),
                        incomming_ray.get_position(1),
                        incomming_ray.get_position(2)};

   float direction[3] = {incomming_ray.get_direction(0),
                         incomming_ray.get_direction(1),
                         incomming_ray.get_direction(2)};

   bool intersect = geometry::intersect_sphere (
                                     position,
                                     direction,
                                     center,
                                     radius);

   float normal[3], intersection[3];
   float new_direction[3];

   if (intersect)
   {
      intersect |= geometry::point_to_sphere (
                                     position,
                                     direction,
                                     center,
                                     radius,
                                     1,
                                     normal,
                                     intersection);

      if (intersect)
      {
          linalg::reflect<float> (direction, normal, new_direction, 3);
          new_ray->set_position (intersection);
          new_ray->set_direction (new_direction);
      }
                                     
   }


   return intersect;

}
