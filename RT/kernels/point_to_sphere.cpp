#include "linalg.h"
#include <cmath>

namespace geo {

/*
 * Function NAME: point_to_sphere
 */
int point_to_sphere (float p[3],
                     float u[3],
                     float c[3],
                     float *v)
{
   float a[3] = { c[0] - p[0],
                  c[1] - p[1],
                  c[2] - p[2] };

   float A = linalg::norm_squared<float> (u, 3);

   float B = -2.0f * linalg::summation<float> (a, 3);

   float C = linalg::norm_squared<float> (a, 3);

   float discriminant = B * B - 4.0f * A * C;

   if (discriminant < 0.0f)
   {
      return -1;
   } else {

      discriminant = sqrtf( discriminant );

      float t = -B - discriminant;

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
