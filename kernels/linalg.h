#ifndef LINALG_H
#define LINALG_H

#include <cmath>

namespace linalg
{

 /*
  * Function NAME: summation
  */
  template <class T>
  T summation (T   *x,
               int  N)
  {
      T sum = (T)0;
      for (int k = 0; k < N; k++)
      {
         sum += x[k];
      }

      return sum;
  }

  /*
   * Function NAME: dot_product
   */
   template <class T>
   T dot_product (T   *a,
                  T   *b,
                  int  N)
  {
 
     float sum = (T)0;
     for (int k = 0; k < N; k++)
     {
        sum += a[k] * b[k];
     }
 
      return sum;
  }
 
 
  /*
   * Function NAME: norm_squared
   */
   template <class T>
  T norm_squared (T *a,
                  T  N)
  {
     return dot_product<T> (a, a, N);
  }
 
 
  /*
   * Function NAME: norm
   */
   template <class T>
   T norm (T *a,
           T  N)
   {
      return (T)sqrt( (double)norm_squared<T> (a, N) );
   }
 
 
  /*
   * Function NAME: unit_vector
   */
   template <class T>
   void unit_vector (T *u,
                     T  N)
   {
      T mag_inverse = (T)1 / norm( u, N );
 
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
   template <class T>
   void project (T   *a,
                 T   *b,
                 T   *c,
                 int  N)
   {
      for (int k = 0; k < N; k++)
      {
         c[k] = a[k];
      }
 
      unit_vector<T> (c, N);
 
      T mag = dot_product<T> (c, b, N);
 
      for (int k = 0; k < N; k++)
      {
         c[k] *= mag;
      }
   }
 
}
 
#endif
