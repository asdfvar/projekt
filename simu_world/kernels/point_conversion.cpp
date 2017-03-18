#include "point_conversion.h"
#include <iostream>
#include <cmath>

/*
** Projects a point onto the plane that is perpendicular to the look direction
** at window distance away. TODO: more description here.
**
** direction is assumed to be unit length on input
*/
float point_conversion( float  perspective[3],
                        float  direction[3],
                        float  window_distance,
                        float  point[3],
                        float *rot,
                        float *output_point )
{

   /*
   ** first step is to get the point (pp) that intersects
   ** the window along the look direction to the point
   */
   float temp;
   float temp_array[3];

   float d_hat[3];
   d_hat[0] = direction[0];
   d_hat[1] = direction[1];
   d_hat[2] = direction[2];

   /*
   ** compute the vector defining the point
   ** relative to the observer
   */
   float p[3];
   p[0] = point[0] - perspective[0];
   p[1] = point[1] - perspective[1];
   p[2] = point[2] - perspective[2];

   float p_norm = sqrtf( p[0] * p[0] +
                         p[1] * p[1] +
                         p[2] * p[2] );

   temp = p[0] * d_hat[0] + p[1] * d_hat[1] + p[2] * d_hat[2];

   // projection of p onto d_hat
   // temp_array = (p dot d_hat) * d_hat
   temp_array[0] = temp * d_hat[0];
   temp_array[1] = temp * d_hat[1];
   temp_array[2] = temp * d_hat[2];

   temp = sqrtf( temp_array[0] * temp_array[0] +
                 temp_array[1] * temp_array[1] +
                 temp_array[2] * temp_array[2] );

   /*
   ** use equality of ratio of right triangles to determine pp_norm
   ** pp_norm = window_distance * p_norm / temp;
   **
   ** ( p_norm factors out later )
   */
   float pp_norm = window_distance / temp;

   /*
   ** pp is co-linear with p. divide out |p| and
   ** multiply by |p|
   **
   ** pp = pp_norm / p_norm * p
   */
   float *pp = output_point;
   pp[0] = pp_norm * p[0];
   pp[1] = pp_norm * p[1];
   pp[2] = pp_norm * p[2];

   /*
   ** then convert that point (pp) to 2-D coordinates as perceived by the
   ** viewer by applying the appropriate rotations to get coordinates
   ** in terms of x,y,depth
   */

   temp_array[0] = pp[0] * rot[0] + pp[1] * rot[1] + pp[2] * rot[2];
   temp_array[1] = pp[0] * rot[3] + pp[1] * rot[4] + pp[2] * rot[5];
   temp_array[2] = pp[0] * rot[6] + pp[1] * rot[7] + pp[2] * rot[8];

   pp[0] = temp_array[0];
   pp[1] = temp_array[1];
   pp[2] = temp_array[2];

   return p_norm;
}

/*
** function name: rotation
**
** Compute the rotation matrix needed for point_conversion
** for rotating the point to the desired location based on the
** observer look direction
*/
void rotation( float  direction[3],
               float* rot )
{
   float d_hat[3] = { direction[0], direction[1], direction[2] };

   float dx  = d_hat[0];
   float dy  = d_hat[1];
   float dz  = d_hat[2];
   float dxy = sqrtf( d_hat[0] * d_hat[0] + d_hat[1] * d_hat[1]);

   /*
   ** rotate the look direction and pp about z by the offset
   ** from the look direction and the y-axis
   **
   ** rot_z = [ dy / |dxy|,  dx / |dxy|, 0 ]
   **         [-dx / |dxy|,  dy / |dxy|, 0 ]
   **         [ 0,           0,          1 ]
   */
   float rot_z[3][3];
   rot_z[0][0] = dy / dxy; rot_z[0][1] = -dx / dxy;
   rot_z[1][0] = dx / dxy; rot_z[1][1] =  dy / dxy;

   // temp_array = rot_z * d_hat;
   float temp_array[3];
   temp_array[0] = rot_z[0][0] * d_hat[0] + rot_z[0][1] * d_hat[1];
   temp_array[1] = rot_z[1][0] * d_hat[0] + rot_z[1][1] * d_hat[1];

   d_hat[0] = temp_array[0];
   d_hat[1] = temp_array[1];

   dy = d_hat[1];
   dz = d_hat[2];

   /* rotate the pp about x by the offset
   ** from the look direction and the x-axis
   **
   ** rot_x = [ 1,  0,        0        ]
   **         [ 0,  dz / |d|, dy / |d| ]
   **         [ 0, -dy / |d|, dz / |d| ]
   */
   float rot_x[3][3];
   rot_x[1][1] = -dz;   rot_x[1][2] = dy;
   rot_x[2][1] = -dy;   rot_x[2][2] = -dz;

   // rot = rot_x * rot_z
   rot[0] = rot_z[0][0];
   rot[1] = rot_z[0][1];
   rot[2] = 0.0f;
   rot[3] = rot_x[1][1] * rot_z[1][0];
   rot[4] = rot_x[1][1] * rot_z[1][1];
   rot[5] = rot_x[1][2];
   rot[6] = rot_x[2][1] * rot_z[1][0];
   rot[7] = rot_x[2][1] * rot_z[1][1];
   rot[8] = rot_x[2][2];

}
