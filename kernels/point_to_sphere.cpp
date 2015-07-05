#include "linalg.h"
#include <iostream>
#include <cmath>

namespace geometry {

/*
 * Function NAME: point_to_sphere
 *
 * Given the vector with the point 'p' with the direction 'u'
 * and the circle with center 'c' and radius 'Radius', this
 * returns the point 'v' where the vector first intersects
 * the circle.
 *
 * The algorithm is derived as follows:
 * d(c, v) = Radius
 * where v = p + u*t
 * and d( ) is the euclidean distance
 *
 * Then squaring both sides and solving for t, v
 * can now be determined.
 *
 * [I ] p - Starting point of the vector
 * [I ] u - Direction of the vector
 * [I ] c - Center of the circle
 * [I ] Radius - Radius of the circle
 * [I ] pick   - value of 1 or 2:
 *                  1: First positive point of intersection
 *                  2: Second positive point of intersection
 * [ O] v      - Resulting point of intersection
 */
int point_to_sphere (float p[3],
                     float u[3],
                     float c[3],
                     float Radius,
                     int   pick,
                     float *v)
{
   float a[3] = { c[0] - p[0],
                  c[1] - p[1],
                  c[2] - p[2] };

   float A  = linalg::norm_squared<float> (u, 3);

   float B  = -2.0f * linalg::summation<float> (a, 3);

   float C  = linalg::norm_squared<float> (a, 3);
         C -= Radius * Radius;

   float discriminant = B * B - 4.0f * A * C;

   if (discriminant < 0.0f)
   {
      return -1;
   } else {

      discriminant = sqrtf( discriminant );

      float t = -B - discriminant;

      if (pick == 1)
      {
         t = -B - discriminant;
      }
      else if (pick == 2)
      {
         t = -B + discriminant;
      }
      else
      {
         std::cout << "pick must be 1 or 2" << std::endl;
         return -1;
      }

      if (t < 0)
      {
          t = -B + discriminant;
      }

      if (t < 0)
      {
         return -1;

      } else {

         t /= 2.0f * A;

         v[0] = p[0] + u[0] * t;
         v[1] = p[1] + u[1] * t;
         v[2] = p[2] + u[2] * t;

         return 0;
      }
   }
}

}
