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
                             float *output_point)
{

   float norm = sqrtf( direction[0] * direction[0] +
                       direction[1] * direction[1] +
                       direction[2] * direction[2]);

   float d_hat[3];
   d_hat[0] = direction[0] / norm;
   d_hat[1] = direction[1] / norm;
   d_hat[2] = direction[2] / norm;

   float xl[3];
   xl[0] = d_hat[0] * window_distance;
   xl[1] = d_hat[1] * window_distance;
   xl[2] = d_hat[2] * window_distance;

   float xp[3];
   xp[0] = point[0] - perspective[0];
   xp[1] = point[1] - perspective[1];
   xp[2] = point[2] - perspective[2];

   norm = sqrtf( xp[0] * xp[0] +
                 xp[1] * xp[1] +
                 xp[2] * xp[2]);

   float xp_hat[3];
   xp_hat[0] = xp[0] / norm;
   xp_hat[1] = xp[1] / norm;
   xp_hat[2] = xp[2] / norm;

   float dot = xp_hat[0] * d_hat[0] +
               xp_hat[1] * d_hat[1] +
               xp_hat[2] * d_hat[2];

   float denom_vec[3];
   denom_vec[0] = dot * d_hat[0];
   denom_vec[1] = dot * d_hat[1];
   denom_vec[2] = dot * d_hat[2];

   float k;
   float max = denom_vec[0];
   if (max < denom_vec[1]) { max = denom_vec[1]; k = xl[1] / denom_vec[1]; }
   if (max < denom_vec[2]) { max = denom_vec[2]; k = xl[1] / denom_vec[2]; }
   else { k = xl[0] / denom_vec[0]; }

   float xp_prime[3];
   xp_prime[0] = k * xp_hat[0];
   xp_prime[1] = k * xp_hat[1];
   xp_prime[2] = k * xp_hat[2];

   float dx  = d_hat[0];
   float dy  = d_hat[1];
   float dxy = sqrtf( d_hat[0] * d_hat[0] + d_hat[1] * d_hat[1]);

   /*
   ** rot_z = [ dy / |dxy|, -dx / |dxy|, 0 ]
   **         [ dx / |dxy|,  dy / |dxy|, 0 ]
   **         [ 0,           0,          1 ]
   */
   float rot_z[3][3];
   rot_z[0][0] = dy / dxy; rot_z[0][1] = -dx / dxy; rot_z[0][2] = 0.0f;
   rot_z[1][0] = dx / dxy; rot_z[1][1] = -dy / dxy; rot_z[1][2] = 0.0f;
   rot_z[2][0] = 0.0f;     rot_z[2][1] = 0.0f;      rot_z[2][2] = 1.0f;

   /*
   ** rot_x = [ 1,           0,           0        ]
   **         [ 0,           dz / |d|,    |dxy|    ]
   **         [ 0,          -|dxy| / |d|, dz / |d| ]
   */
   float rot_x[3][3];
   rot_x[0][0] = 1.0f;     rot_x[0][1] = -dx / dxy; rot_x[0][2] = 0.0f;
   rot_x[1][0] = dx / dxy; rot_x[1][1] = -dy / dxy; rot_x[1][2] = 0.0f;
   rot_x[2][0] = 0.0f;     rot_x[2][1] = 0.0f;      rot_x[2][2] = 1.0f;

   /*
   ** rot_w = [ dy / |dxy|, -dx / |dxy|]
   **         [ dx / |dxy|,  dy / |dxy|]
   */
   float rot_w[2][2];
   rot_w[0][0] =  cosf(rotation); rot_w[0][1] = sinf(rotation);
   rot_w[1][0] = -sinf(rotation); rot_w[1][1] = cosf(rotation);

   float temp[3];
   temp[0] = rot_z[0][0] * xp_prime[0] + rot_z[0][1] * xp_prime[1] + rot_z[0][2] * xp_prime[2];
   temp[1] = rot_z[1][0] * xp_prime[0] + rot_z[1][1] * xp_prime[1] + rot_z[1][2] * xp_prime[2];
   temp[2] = rot_z[2][0] * xp_prime[0] + rot_z[2][1] * xp_prime[1] + rot_z[2][2] * xp_prime[2];

   xp_prime[0] = rot_x[0][0] * temp[0] + rot_x[0][1] * temp[1] + rot_x[0][2] * temp[2];
   xp_prime[1] = rot_x[1][0] * temp[0] + rot_x[1][1] * temp[1] + rot_x[1][2] * temp[2];
   xp_prime[2] = temp[2];

   temp[0] = rot_w[0][0] * xp_prime[0] + rot_w[0][1] * xp_prime[1];
   temp[1] = rot_w[1][0] * xp_prime[0] + rot_w[1][1] * xp_prime[1];

   xp_prime[0] = temp[0];
   xp_prime[1] = temp[1];

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

   // iterate through all the blocks
   for (int block_ind = 0; block_ind < number_of_blocks; block_ind++)
   {

      // get absolute position of block from map
      float block_position[3];
      if (!map->get_position( block_position, block_ind)) continue;

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
   
         glBegin(GL_POLYGON);
           glNormal3f(0.0f, 0.0f, -brightness);
           glVertex3f(view_x[0], view_y[0], dist_ratio[0]);
           glVertex3f(view_x[1], view_y[1], dist_ratio[1]);
           glVertex3f(view_x[2], view_y[2], dist_ratio[2]);
           glVertex3f(view_x[3], view_y[3], dist_ratio[3]);
         glEnd();
      } // for (int face = 0; face < 6; face++)

   }

}
