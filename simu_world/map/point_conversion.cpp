#include "point_conversion.h"
#include <cmath>

void point_conversion(float perspective[3],
                      float direction[3],
                      float rotation,
                      float window_distance,
                      float point[3],
                      float *output_point)
{

   float temp;
   float temp_array[3];

   float d_norm = sqrtf( direction[0] * direction[0] +
                         direction[1] * direction[1] +
                         direction[2] * direction[2]);

   float d_hat[3];
   d_hat[0] = direction[0] / d_norm;
   d_hat[1] = direction[1] / d_norm;
   d_hat[2] = direction[2] / d_norm;

   float p[3];
   p[0] = point[0] - perspective[0];
   p[1] = point[1] - perspective[1];
   p[2] = point[2] - perspective[2];

   float p_norm = sqrtf( p[0] * p[0] +
                         p[1] * p[1] +
                         p[2] * p[2]);

   temp = p[0] * d_hat[0] +
          p[1] * d_hat[1] +
          p[2] * d_hat[2];

   temp_array[0] = temp * d_hat[0];
   temp_array[1] = temp * d_hat[1];
   temp_array[2] = temp * d_hat[2];

   temp = sqrtf( temp_array[0] * temp_array[0] +
                 temp_array[1] * temp_array[1] +
                 temp_array[2] * temp_array[2]);

   float pp_norm = d_norm * p_norm / temp;

   float pp[3];
   pp[0] = pp_norm / p_norm * p[0];
   pp[1] = pp_norm / p_norm * p[1];
   pp[2] = pp_norm / p_norm * p[2];

   float dx  = d_hat[0];
   float dy  = d_hat[1];
   float dz  = d_hat[2];
   float dxy = sqrtf( d_hat[0] * d_hat[0] + d_hat[1] * d_hat[1]);

   /*
   ** rot_z = [ dy / |dxy|,  dx / |dxy|, 0 ]
   **         [-dx / |dxy|,  dy / |dxy|, 0 ]
   **         [ 0,           0,          1 ]
   */
   float rot_z[3][3];
   rot_z[0][0] = dy / dxy; rot_z[0][1] = -dx / dxy; rot_z[0][2] = 0.0f;
   rot_z[1][0] = dx / dxy; rot_z[1][1] =  dy / dxy; rot_z[1][2] = 0.0f;
   rot_z[2][0] = 0.0f;     rot_z[2][1] = 0.0f;      rot_z[2][2] = 1.0f;

   /*
   ** rot_x = [ 1,           0,           0        ]
   **         [ 0,           dz / |d|,    |dxy|    ]
   **         [ 0,          -|dxy| / |d|, dz / |d| ]
   */
   float rot_x[3][3];
   rot_x[0][0] = 1.0f; rot_x[0][1] = 0.0f; rot_x[0][2] = 0.0f;
   rot_x[1][0] = 0.0f; rot_x[1][1] = dz;   rot_x[1][2] = dxy;
   rot_x[2][0] = 0.0f; rot_x[2][1] = dxy;  rot_x[2][2] = dz;

   /*
   ** rot_w = [ dy / |dxy|, -dx / |dxy|]
   **         [ dx / |dxy|,  dy / |dxy|]
   */
   float rot_w[2][2];
   rot_w[0][0] =  cosf(rotation); rot_w[0][1] = sinf(rotation);
   rot_w[1][0] = -sinf(rotation); rot_w[1][1] = cosf(rotation);

   temp_array[0] = rot_z[0][0] * pp[0] + rot_z[0][1] * pp[1] + rot_z[0][2] * pp[2];
   temp_array[1] = rot_z[1][0] * pp[0] + rot_z[1][1] * pp[1] + rot_z[1][2] * pp[2];
   temp_array[2] = rot_z[2][0] * pp[0] + rot_z[2][1] * pp[1] + rot_z[2][2] * pp[2];

   pp[0] = rot_x[0][0] * temp_array[0] + rot_x[0][1] * temp_array[1] + rot_x[0][2] * temp_array[2];
   pp[1] = rot_x[1][0] * temp_array[0] + rot_x[1][1] * temp_array[1] + rot_x[1][2] * temp_array[2];
   pp[2] = rot_x[2][0] * temp_array[0] + rot_x[2][1] * temp_array[1] + rot_x[2][2] * temp_array[2];

   temp_array[0] = rot_w[0][0] * pp[0] + rot_w[0][1] * pp[1];
   temp_array[1] = rot_w[1][0] * pp[0] + rot_w[1][1] * pp[1];

   pp[0] = temp_array[0];
   pp[1] = temp_array[1];

   output_point[0] = pp[0];
   output_point[1] = pp[1];

   return;
}
