#include "linalg.h"

namespace geometry {

/*
 * Function NAME: intersect_sphere
 */
bool intersect_sphere (
               float p[3],
               float u[3],
               float center[3],
               float radius)
{

   float A = linalg::norm_squared <float> (u, 3);

   float B = linalg::dot_product <float> (u, p,      3) -
             linalg::dot_product <float> (u, center, 3);

   float t = -B / A;

   float v[3];

   v[0] = p[0] + u[0] * t;
   v[1] = p[1] + u[1] * t;
   v[2] = p[2] + u[2] * t;

   float dist[3];

   dist[0] = v[0] - center[0];
   dist[1] = v[1] - center[1];
   dist[2] = v[2] - center[2];

  float length = linalg::norm <float> (dist, 3);

  if (length <= radius)
  {
     return true;
  }
  else
  {
     return false;
  }

}

} // namespace geometry
