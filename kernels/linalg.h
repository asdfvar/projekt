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
   ** Function NAME: cross_product
   ** c = a x b
   */

   template <class T>
   void cross_product (T *c,
                       T *a,
                       T *b)
   {
      c[0] = a[1]*b[2] - a[2]*b[1];
      c[1] = a[2]*b[0] - a[0]*b[2];
      c[2] = a[0]*b[1] - a[1]*b[0];
   }
 
  /*
   * Function NAME: project
   *
   * c = Project b onto a
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

   /*
   ** Function NAME: reflect
   **
   ** reflected = reflection of u off of normal
   */
   template <class T>
   void reflect (T *u,
                 T *normal,
                 T *reflected,
                 int N)
   {
      T two = (T)2;
      project<T> (normal, u, reflected, N);
      for (int k = 0; k < N; k++) reflected[k] *= two;
      for (int k = 0; k < N; k++) reflected[k] = u[k] - reflected[k];
   }

   /*
   ** Function NAME: linear_transform
   **
   ** apply the linear tranform 'Matrix' to
   ** the input 'input' of dimension 'dim_in'
   ** to produce an output of dimension 'dim_out'
   */
   template <class T>
   void linear_transform (T *input,
                          T *Matrix,
                          T *output,
                          int dim_in,
                          int dim_out)
   {
      int k = 0;
      for (int i = 0; i < dim_out; i++)
      {

         output[i] = 0.0f;

         for (int j = 0; j < dim_in; j++, k++)
         {
            output[i] += input[j] * Matrix[k];
         }
      }
   }

   /*
   ** Function NAME: angle_offset
   **
   ** Compute the angle offset between two vectors
   ** 'vec1' and 'vec2' of dimension 'N'
   **
   ** RESTRICTION:
   ** whatever restriction is imposed by acos from cmath
   */
   template <class T>
   T angle_offset (T *vec1,
                   T *vec2,
                   int N)
   {
      T num   = linalg::dot_product<T>( vec1, vec2, N);
      T den   = linalg::norm<T>(vec1, N) * linalg::norm<T>(vec2, N);
      T costh = num / den;
      T theta = acos(costh);

      return theta;
   }
 
}
 
#endif
