#include <cmath>
#include "linalg.h"
#include "shape.h"
#include "sphere.h"
#include <iostream>

#define EPS 0.00001f

/*
 * Constructor NAME: sphere
 */
Sphere::Sphere ( float center_in[3],
                 float radius_in,
                 float color_in[3])
               : Shape (center_in,
                        color_in)
{
   radius = radius_in;
}

/*
** Function NAME: intersect
*/
bool Sphere::intersect ( Ray    incomming_ray,
                         Ray   *new_ray,
                         float *distance,
                         float  color_intensities[],
                         float **reflection_table_x_in,
                         float **reflection_table_y_in,
                         int   *reflection_table_N_in)
{

   color_intensities[0] = color[0];
   color_intensities[1] = color[1];
   color_intensities[2] = color[2];

  *reflection_table_x_in = reflection_table_x;
  *reflection_table_y_in = reflection_table_y;
  *reflection_table_N_in = reflection_table_N;

  *distance = -1.0f;

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
      intersect = geometry::point_to_sphere (
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

         *distance = sqrt((position[0] - intersection[0]) * (position[0] - intersection[0]) +
                          (position[1] - intersection[1]) * (position[1] - intersection[1]) +
                          (position[2] - intersection[2]) * (position[2] - intersection[2]));

         if (*distance < EPS)
         {
           *distance = -1.0f;
            intersect = false;
         }

      }
   }

   return intersect;

}
