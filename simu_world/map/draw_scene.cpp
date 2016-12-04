#include "draw_scene.h"
#include "user.h"
#include "map.h"
#include "point_conversion.h"
#include "linalg.h"
#include <cmath>

/*
** OpenGL includes
*/
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

/*
** function: draw_scene
**
** The coordinates are converted so that they are relative
** to the user.
*/
void draw_scene( User *user, Map  *map)
{

   float user_direction[3];

   user->get_direction( user_direction);

//<<<
std::cout << __FILE__ << ":user direction = " << user_direction[0] << ", " << user_direction[1] << ", " << user_direction[2] << std::endl;
//>>>

   float horizontal_length = sqrtf( user_direction[0] * user_direction[0] +
                                    user_direction[1] * user_direction[1]);
   float vert_angle        = asinf( user_direction[2] / horizontal_length);
   float hor_angle         = asinf( user_direction[1] / user_direction[0]);

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

      float output_point_distance[4];
      float output_point[3];
      float view_x[4];
      float view_y[4];
      bool  valid_view[4];

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

            float user_position[3];
            user->get_position( user_position);

            // get position of block corner relative to user but at absolute direction
            user_to_corner_x[corner] = corner_pos_x[corner] - user_position[0];
            user_to_corner_y[corner] = corner_pos_y[corner] - user_position[1];
            user_to_corner_z[corner] = corner_pos_z[corner] - user_position[2];
      
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
            user_to_corner_x[corner] = xp * cosf(vert_angle) - zp * sinf(vert_angle);
            user_to_corner_y[corner] = yp;
            user_to_corner_z[corner] = xp * sinf(vert_angle) + zp * cosf(vert_angle);
   
            float corner_point[3] = { corner_pos_x[corner],
                                      corner_pos_y[corner],
                                      corner_pos_z[corner] };

            float window_distance = user->get_window_distance();

            output_point_distance[corner] = point_conversion(user_position,
                                                             user_direction,
                                                             window_distance,
                                                             corner_point,
                                                             output_point);

            float window_width = user->get_window_width();

            // scale to [-1, 1]
            view_x[corner] = output_point[0] / window_width;
            view_y[corner] = output_point[1] / window_width;
            valid_view[corner] = (output_point[2] < 0.0f);

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
         float to_user[3] = {-user_direction[0], -user_direction[1], -user_direction[2]};
         linalg::unit_vector<float>  ( normal, 3);
         float brightness = linalg::dot_product<float>  ( normal, to_user, 3);

         if (valid_view[0] && valid_view[1] && valid_view[2] && valid_view[3])
         {
            float max_view_distance = 100.0f;
            glBegin(GL_POLYGON);
              glNormal3f(0.0f, 0.0f, -brightness);
              glVertex3f(view_x[0], view_y[0], output_point_distance[0] / max_view_distance);
              glVertex3f(view_x[1], view_y[1], output_point_distance[1] / max_view_distance);
              glVertex3f(view_x[2], view_y[2], output_point_distance[2] / max_view_distance);
              glVertex3f(view_x[3], view_y[3], output_point_distance[3] / max_view_distance);
            glEnd();
         }

      } // for (int face = 0; face < 6; face++)

   }

}
