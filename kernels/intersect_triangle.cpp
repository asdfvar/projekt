#include "geometry.h"
#include "linalg.h"

#define EPS 0.00001f

namespace geometry {

bool intersect_triangle (
        /* [I ] */        float line_position[3],
        /* [I ] */        float line_direction[3],
        /* [I ] */        float triangle_pos_0[3],
        /* [I ] */        float triangle_pos_1[3],
        /* [I ] */        float triangle_pos_2[3],
        /* [ O] */        float *intersection,
        /* [ O] */        float *normal)
{

   float p[3], q[3], r[3];

   for (int k = 0; k < 3; k++) p[k] = triangle_pos_1[k] - triangle_pos_0[k];
   for (int k = 0; k < 3; k++) q[k] = triangle_pos_2[k] - triangle_pos_1[k];
   for (int k = 0; k < 3; k++) r[k] = triangle_pos_0[k] - triangle_pos_2[k];

   linalg::cross_product<float> (normal, p, q);

   /*
   ** Test if the line is parallel with the triangle. If it is,
   ** return false.
   */
   float dot = linalg::dot_product<float> (line_direction, normal, 3);
   if (dot < -EPS || dot > EPS)
   {
      return false;
   }

   float xl0[3];

   for (int k = 0; k < 3; k++) xl0[k] = line_position[k] - triangle_pos_0[k];

   float numerator = linalg::dot_product<float> (normal, xl0, 3);
   float denominator = linalg::dot_product<float> (normal, line_direction, 3);

   float tv = numerator / denominator;

   for (int k = 0; k < 3; k++) intersection[k] = line_position[k] + line_direction[k] * tv;

   /*
   ** Test if the line intersects the triangle
   */

   float x0v[3], x1v[3], x2v[3];

   for (int k = 0; k < 3; k++) x0v[k] = intersection[k] - triangle_pos_0[k];
   for (int k = 0; k < 3; k++) x1v[k] = intersection[k] - triangle_pos_1[k];
   for (int k = 0; k < 3; k++) x2v[k] = intersection[k] - triangle_pos_2[k];

   float a[3], b[3], c[3];

   linalg::cross_product<float> (a, p, x0v);
   linalg::cross_product<float> (b, q, x1v);
   linalg::cross_product<float> (c, r, x2v);

   float a_dot_n = linalg::dot_product<float> (a, normal, 3);
   float b_dot_n = linalg::dot_product<float> (b, normal, 3);
   float c_dot_n = linalg::dot_product<float> (c, normal, 3);

   if ( a_dot_n * b_dot_n >= 0.0f && a_dot_n * c_dot_n >= 0.0f )
   {
      return true;
   }
   else
   {
      return false;
   }
}

} // namspace geometry
