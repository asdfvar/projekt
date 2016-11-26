#include <cmath>
#include "user.h"
#include "linalg.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

static void point_conversion(float perspective[3],
                             float direction[3],
                             float rotation,
                             float window_distance,
                             float point[3],
                             bool  debug,
                             float *output_point)
{

   float temp;
   float temp_array[3];

   float d_norm = sqrtf( direction[0] * direction[0] +
                         direction[1] * direction[1] +
                         direction[2] * direction[2]);

if (debug) std::cout << "window distance = " << window_distance << std::endl;
if (debug) std::cout << "point = " << point[0] << ", " << point[1] << ", " << point[2] << std::endl;

   float d_hat[3];
   d_hat[0] = direction[0] / d_norm;
   d_hat[1] = direction[1] / d_norm;
   d_hat[2] = direction[2] / d_norm;

   float d[3];
   d[0] = d_hat[0] * window_distance;
   d[1] = d_hat[1] * window_distance;
   d[2] = d_hat[2] * window_distance;

   float p[3];
   p[0] = point[0] - perspective[0];
   p[1] = point[1] - perspective[1];
   p[2] = point[2] - perspective[2];

   float p_norm = sqrtf( p[0] * p[0] +
                         p[1] * p[1] +
                         p[2] * p[2]);

   float p_hat[3];
   p_hat[0] = p[0] / p_norm;
   p_hat[1] = p[1] / p_norm;
   p_hat[2] = p[2] / p_norm;

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

if (debug) std::cout << "pp = " << pp[0] << ", " << pp[1] << ", " << pp[2] << std::endl;

#if 1
   float dot = p_hat[0] * d_hat[0] +
               p_hat[1] * d_hat[1] +
               p_hat[2] * d_hat[2];

   float denom_vec[3];
   denom_vec[0] = dot * d_hat[0];
   denom_vec[1] = dot * d_hat[1];
   denom_vec[2] = dot * d_hat[2];

   float k;
   float max = denom_vec[0];
   if (max < denom_vec[1]) { max = denom_vec[1]; k = d[1] / denom_vec[1]; }
   if (max < denom_vec[2]) { max = denom_vec[2]; k = d[1] / denom_vec[2]; }
   else { k = d[0] / denom_vec[0]; }

   float xp_prime[3];
   xp_prime[0] = k * p_hat[0];
   xp_prime[1] = k * p_hat[1];
   xp_prime[2] = k * p_hat[2];

   float dx  = d_hat[0];
   float dy  = d_hat[1];
   float dz  = d_hat[2];
   float dxy = sqrtf( d_hat[0] * d_hat[0] + d_hat[1] * d_hat[1]);

if (debug) std::cout << "dxy = " << dxy << std::endl;

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

if (debug) std::cout << "rot(z) = " << temp_array[0] << ", " << temp_array[1] << ", " << temp_array[2] << std::endl;

   pp[0] = rot_x[0][0] * temp_array[0] + rot_x[0][1] * temp_array[1] + rot_x[0][2] * temp_array[2];
   pp[1] = rot_x[1][0] * temp_array[0] + rot_x[1][1] * temp_array[1] + rot_x[1][2] * temp_array[2];
   pp[2] = rot_x[2][0] * temp_array[0] + rot_x[2][1] * temp_array[1] + rot_x[2][2] * temp_array[2];

if (debug) std::cout << "rot(x) = " << pp[0] << ", " << pp[1] << ", " << pp[2] << std::endl;

   temp_array[0] = rot_w[0][0] * pp[0] + rot_w[0][1] * pp[1];
   temp_array[1] = rot_w[1][0] * pp[0] + rot_w[1][1] * pp[1];

   pp[0] = temp_array[0];
   pp[1] = temp_array[1];

   output_point[0] = pp[0];
   output_point[1] = pp[1];
#endif

   return;
}

User::User( void)
{
   /*
   ** User positional attributes
   */
   position[0] = 0.0f;
   position[1] = 0.0f;
   position[2] = 0.0f;

   direction[0] = 1.0f;
   direction[1] = 0.0f;
   direction[2] = 0.0f;

   speed = 0.0f;

   /*
   ** User view-window attributes
   */
   window_distance = 1.0f;
   window_width    = 1.0f;
   window_height   = 1.0f;

}

void User::update( float dt)
{
   position[0] += direction[0] * speed * dt;
   position[1] += direction[1] * speed * dt;
   position[2] += direction[2] * speed * dt;
}

void User::move_upward( void)
{
   position[2] += 0.5f;
}

void User::move_downward( void)
{
   position[2] -= 0.5f;
}

void User::move_right( void)
{
   position[1] += 0.5f;
}

void User::move_left( void)
{
   position[1] -= 0.5f;
}

void User::move_forward( void)
{
   speed = 1.0f;
}

void User::move_backward( void)
{
   speed = -0.3f;
}

void User::stop( void)
{
   speed = 0.0f;
}

/*
** function: draw_scene from: User
**
** The coordinates are converted so that they are relative
** to the user.
*/
void User::draw_scene( Map *map)
{

   // get vertical angle of look direction
   float horizontal_length = sqrtf( direction[0]*direction[0] + direction[1]*direction[1]);
   float vert_angle = asinf( direction[2] / horizontal_length);
   float hor_angle  = asinf( direction[1] / direction[0]);

   int number_of_blocks = map->get_dimensions();

//<<<
number_of_blocks = 1;
//>>>

   // iterate through all the blocks
   for (int block_ind = 0; block_ind < number_of_blocks; block_ind++)
   {

      // get absolute position of block from map
      float block_position[3];
#if 0
      if (!map->get_position( block_position, block_ind)) continue;
#else
//<<<
map->get_position( block_position, block_ind);
//>>>
#endif

//<<<
block_position[0] = 4.0f;
block_position[1] = 0.0f;
block_position[2] = 0.0f;
//>>>

      // get 4 positions of the face
      float corner_pos_x[4];
      float corner_pos_y[4];
      float corner_pos_z[4];

      float user_to_corner_x[4];
      float user_to_corner_y[4];
      float user_to_corner_z[4];

      float normal[3];

      float view_x[4] = {0.0f, 0.0f, 0.0f, 0.0f};
      float view_y[4] = {0.0f, 0.0f, 0.0f, 0.0f};
      float view_depth[4] = {0.0f, 0.0f, 0.0f, 0.0f};

      float new_view_x[4];
      float new_view_y[4];

      for (int face = 0; face < 6; face++)
      {
         for (int corner = 0; corner < 4; corner++)
         {
            if (face == 0) { // x+
               if (corner == 0)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 1)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 2)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 3)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
            } else if (face == 1) { // x-
               if (corner == 0)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 1)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 2)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 3)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
            } else if (face == 2) { // y+
               if (corner == 0)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 1)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 2)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 3)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
            } else if (face == 3) { // y-
               if (corner == 0)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 1)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 2)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 3)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
            } else if (face == 4) { // z+
               if (corner == 0)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 1)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 2)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
               else if (corner == 3)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] + 0.5f;
               }
            } else if (face == 5) { // z-
               if (corner == 0)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 1)
               {
                  corner_pos_x[corner] = block_position[0] - 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 2)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] + 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
               else if (corner == 3)
               {
                  corner_pos_x[corner] = block_position[0] + 0.5f;
                  corner_pos_y[corner] = block_position[1] - 0.5f;
                  corner_pos_z[corner] = block_position[2] - 0.5f;
               }
            }

            // get position of block corner relative to user but at absolute direction
            user_to_corner_x[corner] = corner_pos_x[corner] - position[0];
            user_to_corner_y[corner] = corner_pos_y[corner] - position[1];
            user_to_corner_z[corner] = corner_pos_z[corner] - position[2];
      
            // rotate to user perspective to get relative orientation
            /*
            ** [ cos(hor_angle) -sin(hor_angle) 0 ]
            ** [ sin(hor_angle)  cos(hor_angle) 0 ]
            ** [ 0               0              1 ]
            */
            float xp = user_to_corner_x[corner]*cosf(hor_angle) - user_to_corner_y[corner]*sinf(hor_angle);
            float yp = user_to_corner_x[corner]*sinf(hor_angle) + user_to_corner_y[corner]*cosf(hor_angle);
            float zp = user_to_corner_z[corner];
      
            /*
            ** [ cos(vert_angle) 0 -sin(vert_angle) ]
            ** [ 0               1  0               ]
            ** [ sin(vert_angle) 0  cos(vert_angle) ]
            */
            user_to_corner_x[corner] = xp*cosf(vert_angle) - zp*sinf(vert_angle);
            user_to_corner_y[corner] = yp;
            user_to_corner_z[corner] = xp*sinf(vert_angle) + zp*cosf(vert_angle);
   
            float view_window_ratio_up    = 0.5f * window_height / window_distance;
            float corner_ratio_up         = user_to_corner_z[corner] / user_to_corner_x[corner];
            view_y[corner]                = corner_ratio_up / view_window_ratio_up * window_height;
            float view_window_ratio_right = 0.5f * window_width / window_distance;
            float corner_ratio_right      = user_to_corner_y[corner] / user_to_corner_x[corner];
            view_x[corner]                = corner_ratio_right / view_window_ratio_right * window_width;
            view_depth[corner]            = sqrtf( user_to_corner_x[corner] * user_to_corner_x[corner] +
                                                   user_to_corner_y[corner] * user_to_corner_y[corner] +
                                                   user_to_corner_z[corner] * user_to_corner_z[corner]);

            float corner_point[3] = { corner_pos_x[corner],
                                      corner_pos_y[corner],
                                      corner_pos_z[corner] };
//<<<
// std::cout << "corner = " << corner << std::endl;
//>>>
            float output_point[2];
            point_conversion(position,
                             direction,
                             0.0f, /* rotation (w) */
                             window_distance,
                             corner_point,
                             (face == 0 && corner == 0),
                             output_point);
#if 1
            // scale to [-1, 1]
            new_view_x[corner] = output_point[0] / window_width;
            new_view_y[corner] = output_point[1] / window_width;
#endif

         } // for (int corner = 0; corner < 4; corner++)

         // get relative normal direction
         float vec1[3] = {user_to_corner_x[1] - user_to_corner_x[0],
                          user_to_corner_y[1] - user_to_corner_y[0],
                          user_to_corner_z[1] - user_to_corner_z[0]};
         float vec2[3] = {user_to_corner_x[3] - user_to_corner_x[0],
                          user_to_corner_y[3] - user_to_corner_y[0],
                          user_to_corner_z[3] - user_to_corner_z[0]};

         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         float to_user[3] = {-direction[0], -direction[1], -direction[2]};
         linalg::unit_vector<float>  ( normal, 3);
         float brightness = linalg::dot_product<float>  ( normal, to_user, 3);

         // set max distance to 100.0
         float dist_ratio[4];
         for (int ind = 0; ind < 4; ind++) dist_ratio[ind] = view_depth[ind] / 100.0f;

//<<<
std::cout << "face = " << face << std::endl;
for (int k = 0; k < 4; k++) {
std::cout << "k = " << k << ": view = (" <<  view_x[k] << ", " << view_y[k] << ")";
std::cout << ":\tnew view = (" << new_view_x[k] << ", " << new_view_y[k] << ")" << std::endl;
}
std::cout << std::endl;
//>>>
   
         glBegin(GL_POLYGON);
           glNormal3f(0.0f, 0.0f, -brightness);
           glVertex3f(new_view_x[0], new_view_y[0], dist_ratio[0]);
           glVertex3f(new_view_x[1], new_view_y[1], dist_ratio[1]);
           glVertex3f(new_view_x[2], new_view_y[2], dist_ratio[2]);
           glVertex3f(new_view_x[3], new_view_y[3], dist_ratio[3]);
         glEnd();
      } // for (int face = 0; face < 6; face++)

   }

}
