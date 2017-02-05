#include "draw_block.h"
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

void draw_block( float  block_position[3],
                 float *color,
                 float *vertices,
                 User  *user)
{

   int vertex_ind = 0;

   float user_direction[3];
   user->get_direction( user_direction);

   float user_position[3];
   user->get_position( user_position);

   float user_to_block[3] = {block_position[0] - user_position[0],
                             block_position[1] - user_position[1],
                             block_position[2] - user_position[2]};

   /*
   ** Return if the user direction and block position are > 90 degrees off
   */
   if ( linalg::dot_product<float>( user_direction, user_to_block, 3) < 0.0f)
   {
      return;
   }

   // get 4 positions of the face
   float corner_pos_x[4];
   float corner_pos_y[4];
   float corner_pos_z[4];

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
         if (face == 0)
         { // x+
            if (corner == 0)
            {
               corner_pos_x[corner] = block_position[0] + 0.5f;
               corner_pos_y[corner] = block_position[1] + 0.5f;
               corner_pos_z[corner] = block_position[2] + 0.5f;
            }
            else if (corner == 1)
            {
               corner_pos_x[corner] = block_position[0] + 0.5f;
               corner_pos_y[corner] = block_position[1] - 0.5f;
               corner_pos_z[corner] = block_position[2] + 0.5f;
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
               corner_pos_y[corner] = block_position[1] + 0.5f;
               corner_pos_z[corner] = block_position[2] - 0.5f;
            }
         } else if (face == 1)
         { // x-
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
               corner_pos_x[corner] = block_position[0] + 0.5f;
               corner_pos_y[corner] = block_position[1] + 0.5f;
               corner_pos_z[corner] = block_position[2] + 0.5f;
            }
            else if (corner == 2)
            {
               corner_pos_x[corner] = block_position[0] + 0.5f;
               corner_pos_y[corner] = block_position[1] + 0.5f;
               corner_pos_z[corner] = block_position[2] - 0.5f;
            }
            else if (corner == 3)
            {
               corner_pos_x[corner] = block_position[0] - 0.5f;
               corner_pos_y[corner] = block_position[1] + 0.5f;
               corner_pos_z[corner] = block_position[2] - 0.5f;
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
               corner_pos_x[corner] = block_position[0] + 0.5f;
               corner_pos_y[corner] = block_position[1] - 0.5f;
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
               corner_pos_x[corner] = block_position[0] - 0.5f;
               corner_pos_y[corner] = block_position[1] + 0.5f;
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

         float window_distance = user->get_window_distance();

         float corner_point[3] = { corner_pos_x[corner],
                                   corner_pos_y[corner],
                                   corner_pos_z[corner] };

         output_point_distance[corner] = point_conversion( user_position,
                                                           user_direction,
                                                           window_distance,
                                                           corner_point,
                                                           output_point);

         float window_width = user->get_window_width();

         // scale to [-1, 1]
         view_x[corner]     = output_point[0] / window_width;
         view_y[corner]     = output_point[1] / window_width;
         valid_view[corner] = (output_point[2] < 0.0f);

      } // for (int corner = 0; corner < 4; corner++)

      float vec1[3] = {corner_pos_x[1] - corner_pos_x[0],
                       corner_pos_y[1] - corner_pos_y[0],
                       corner_pos_z[1] - corner_pos_z[0]};
      float vec2[3] = {corner_pos_x[3] - corner_pos_x[0],
                       corner_pos_y[3] - corner_pos_y[0],
                       corner_pos_z[3] - corner_pos_z[0]};

      linalg::cross_product<float>( normal, vec2, vec1);
      linalg::unit_vector<float>  ( normal, 3);
      float user_opposite[3] = {-user_direction[0], -user_direction[1], -user_direction[2]};
      linalg::unit_vector<float>  ( user_opposite, 3);
      float brightness = linalg::dot_product<float>( normal, user_opposite, 3);

      if (valid_view[0] && valid_view[1] && valid_view[2] && valid_view[3])
      {

        ogl::draw_polygon( view_x,
                           view_y,
                           output_point_distance,
                           color,
                           brightness,
                           4);
      }

      vertices[vertex_ind++] = corner_pos_x[0];
      vertices[vertex_ind++] = corner_pos_y[0];
      vertices[vertex_ind++] = corner_pos_z[0];

      vertices[vertex_ind++] = corner_pos_x[1];
      vertices[vertex_ind++] = corner_pos_y[1];
      vertices[vertex_ind++] = corner_pos_z[1];

      vertices[vertex_ind++] = corner_pos_x[2];
      vertices[vertex_ind++] = corner_pos_y[2];
      vertices[vertex_ind++] = corner_pos_z[2];

      vertices[vertex_ind++] = corner_pos_x[0];
      vertices[vertex_ind++] = corner_pos_y[0];
      vertices[vertex_ind++] = corner_pos_z[0];

      vertices[vertex_ind++] = corner_pos_x[2];
      vertices[vertex_ind++] = corner_pos_y[2];
      vertices[vertex_ind++] = corner_pos_z[2];

      vertices[vertex_ind++] = corner_pos_x[3];
      vertices[vertex_ind++] = corner_pos_y[3];
      vertices[vertex_ind++] = corner_pos_z[3];

   } // for (int face = 0; face < 6; face++)

}
