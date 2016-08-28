#include <cmath>
#include "user.h"
#include "linalg.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

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

void User::move_forward( void)
{
   speed = 0.3f;
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

      float view_x[4];
      float view_y[4];

      for (int corner = 0; corner < 4; corner++)
      {
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

         float view_window_ratio_up = 0.5f * window_height / window_distance;
         float corner_ratio_up = user_to_corner_z[corner] / user_to_corner_x[corner];
         view_y[corner] = corner_ratio_up / view_window_ratio_up * window_height;
         float view_window_ratio_right = 0.5f * window_width / window_distance;
         float corner_ratio_right = user_to_corner_y[corner] / user_to_corner_x[corner];
         view_x[corner] = corner_ratio_right / view_window_ratio_right * window_width;
      }

      // check if any point falls within the viewing window. If so, then render it
//      linalg::cross_product<float>(c, a, b); // c = a x b

      glBegin(GL_POLYGON);
#if 1
         glVertex3f(view_x[0], view_y[0], 0.0f);
         glVertex3f(view_x[1], view_y[1], 0.0f);
         glVertex3f(view_x[2], view_y[2], 0.0f);
         glVertex3f(view_x[3], view_y[3], 0.0f);
#else
         glNormal3f(1.0f, 1.0f, -0.5f);
         glVertex3f(0.5f, 0.5f,  0.0f);
         glVertex3f(0.2f, 0.5f,  0.0f);
         glVertex3f(0.35f, 0.7f, 0.0f);
#endif
      glEnd();
   }
   
}
