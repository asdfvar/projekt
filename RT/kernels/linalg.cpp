#include <cmath>

namespace linalg
{

 /*
  * Function NAME: summation
  */
 float summation (float *x,
                  int    N)
 {

    float sum = 0.0f;
    for (int k = 0; k < N; k++)
    {
       sum += x[k];
    }

    return sum;
 }

 /*
  * Function NAME: dot_product
  */
 float dot_product (float *a,
                    float *b,
                    int   N)
 {

    float sum = 0.0f;
    for (int k = 0; k < N; k++)
    {
       sum += a[k] * b[k];
    }

     return sum;
 }

 /*
  * Function NAME: norm_squared
  */
 float norm_squared (float *a,
                     int    N)
 {
    return dot_product (a, a, N);
 }

 /*
  * Function NAME: norm
  */
 float norm (float *a,
             int    N)
 {
    return sqrtf( norm_squared (a, N) );
 }

 /*
  * Function NAME: unit_vector
  */
 void unit_vector (float *u,
                   int    N)
 {
    float mag_inverse = 1.0f / norm( u, N );

    for (int k = 0; k < N; k++)
    {
       u[k] *= mag_inverse;
    }
 }

 /*
  * Function NAME: project
  *
  * Project b onto a
  */
  void project (float *a,
                float *b,
                float *c,
                int    N)
 {
    for (int k = 0; k < N; k++)
    {
       c[k] = a[k];
    }

    unit_vector (c, N);

    float mag = dot_product (c, b, N);

    for (int k = 0; k < N; k++)
    {
       c[k] *= mag;
    }
 }

}
