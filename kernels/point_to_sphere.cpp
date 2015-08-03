#include "linalg.h"
#include <iostream>
#include <cmath>

namespace geometry {

/*
 * Function NAME: point_to_sphere
 *
 * Given the vector with the point 'line_position' with the direction 'line_direction'
 * and the circle with center 'center' and radius 'radius', this
 * returns the point 'intersection' where the vector first intersects
 * the circle.
 *
 * The algorithm is derived as follows:
 * d(center, intersection) = radius
 * where intersection = line_position + line_direction*t
 * and d( ) is the euclidean distance
 *
 * Then squaring both sides and solving for t, intersection
 * can now be determined.
 *
 * [I ] line_position - Starting point of the vector
 * [I ] line_direction - Direction of the vector
 * [I ] center - Center of the circle
 * [I ] radius - radius of the circle
 * [I ] pick   - value of 1 or 2:
 *                  1: First positive point of intersection
 *                  2: Second positive point of intersection
 * [ O] intersection      - Resulting point of intersection
 */
bool point_to_sphere (
   /* [I ] */   float line_position[3],
   /* [I ] */   float line_direction[3],
   /* [I ] */   float center[3],
   /* [I ] */   float radius,
   /* [I ] */   int   pick,
   /* [I ] */   float *normal,
   /* [I ] */   float *intersection)
{
   float a[3] = { center[0] - line_position[0],
                  center[1] - line_position[1],
                  center[2] - line_position[2] };

   float A  = linalg::norm_squared<float> (line_direction, 3);

   float B  = -2.0f * linalg::summation<float> (a, 3);

   float C  = linalg::norm_squared<float> (a, 3);
         C -= radius * radius;

   float discriminant = B * B - 4.0f * A * C;

   if (discriminant < 0.0f)
   {
      return false;
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
         return false;
      }

      /*
      ** Often, during refraction, the ray be be interior to the sphere.
      */
      if (t < 0)
      {
          t = -B + discriminant;
      }

      if (t < 0)
      {
         return false;

      } else {

         t /= 2.0f * A;
         for (int k = 0; k < 3; k++) intersection[k] = line_position[k] + line_direction[k] * t;
         for (int k = 0; k < 3; k++) normal[k] = intersection[k] - center[k];

         return true;
      }
   }
}

}
