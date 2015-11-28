#include "shape.h"
#include "plane.h"
#include "ray.h"
#include "geometry.h"

/*
** Constructor NAME: plane
*/
Plane::Plane ( float p1_in[3],
               float p2_in[3],
               float p3_in[3],
               float color_in[3],
               float reflectivity_in)
             : Shape (p1_in,
                      color_in,
                      reflectivity_in)
{

   p1[0] = p1_in[0];
   p1[1] = p1_in[1];
   p1[2] = p1_in[2];

   p2[0] = p2_in[0];
   p2[1] = p2_in[1];
   p2[2] = p2_in[2];

   p3[0] = p3_in[0];
   p3[1] = p3_in[1];
   p3[2] = p3_in[2];

}

bool Plane::intersect(
                 Ray    incomming_ray,
                 Ray   *new_ray,
                 float *normal,
                 float *distance,
                 float color_intensities[],
                 float *reflectivity_out,
                 float **reflection_table_x_in,
                 float **reflection_table_y_in,
                 int   *reflection_table_N_in)
{

   color_intensities[0] = color[0];
   color_intensities[1] = color[1];
   color_intensities[2] = color[2];

  *reflectivity_out = reflectivity;

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

   float intersection[3];

   bool intersect = geometry::intersect_triangle (
                            position,
                            direction,
                            p1, p2, p3,
                            intersection,
                            normal);

   return intersect;
 
}
