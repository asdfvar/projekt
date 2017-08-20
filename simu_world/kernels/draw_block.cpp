#include "draw_block.h"
#include "point_conversion.h"
#include "opengl_interface.h"
#include "linalg.h"
#include <cmath>

#define ENABLE_RENDERING

/*
** function name: draw_block_1
*/
void draw_block_1 (
      float  block_position[3],
      float *user_position,
      float *user_direction,
      float  window_distance,
      float  window_width,
      int    sides,
      float *rot,
      float *color )
{

   if (sides == 0) return;

   int vertex_ind = 0;

   float user_to_block[3] = { block_position[0] - user_position[0],
                              block_position[1] - user_position[1],
                              block_position[2] - user_position[2] };

   /*
   ** Return if the user direction and block position are > 90 degrees off
   */
   if ( linalg::dot_product<float>( user_direction, user_to_block, 3) < 0.0f)
   {
      return;
   }

   float corner_pos_x[4];
   float corner_pos_y[4];
   float corner_pos_z[4];

   float normal[3];
   float vertices_x[4];
   float vertices_y[4];

   float output_point_distance[8];
   float distance[4];
   float output_point[3];
   float view_x[8];
   float view_y[8];
   bool  valid_view[8];

   float corner_point[3];
   float brightness;

   float user_opposite[3] = {-user_direction[0], -user_direction[1], -user_direction[2]};

   float face_xp = block_position[0] + 0.5f;
   float face_xn = block_position[0] - 0.5f;
   float face_yp = block_position[1] + 0.5f;
   float face_yn = block_position[1] - 0.5f;
   float face_zp = block_position[2] + 0.5f;
   float face_zn = block_position[2] - 0.5f;

   float vec1[3];
   float vec2[3];

   // corner 0
   corner_point[0] = face_xp;
   corner_point[1] = face_yp;
   corner_point[2] = face_zp;

   if( sides && (DRAW_FRONT || DRAW_LEFT || DRAW_TOP) )
   {
      output_point_distance[0] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[0]     = output_point[0] / window_width;
   view_y[0]     = output_point[1] / window_width;
   valid_view[0] = ( output_point[2] < 0.0f );

   // corner 1
   corner_point[0] = face_xp;
   corner_point[1] = face_yp;
   corner_point[2] = face_zn;

   if( sides && (DRAW_FRONT || DRAW_LEFT || DRAW_BOTTOM) )
   {
      output_point_distance[1] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[1]     = output_point[0] / window_width;
   view_y[1]     = output_point[1] / window_width;
   valid_view[1] = ( output_point[2] < 0.0f );

   // corner 2
   corner_point[0] = face_xp;
   corner_point[1] = face_yn;
   corner_point[2] = face_zp;

   if( sides && (DRAW_FRONT || DRAW_RIGHT || DRAW_TOP) )
   {
      output_point_distance[2] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[2]     = output_point[0] / window_width;
   view_y[2]     = output_point[1] / window_width;
   valid_view[2] = ( output_point[2] < 0.0f );

   // corner 3
   corner_point[0] = face_xp;
   corner_point[1] = face_yn;
   corner_point[2] = face_zn;

   if( sides && (DRAW_FRONT || DRAW_RIGHT || DRAW_BOTTOM) )
   {
      output_point_distance[3] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[3]     = output_point[0] / window_width;
   view_y[3]     = output_point[1] / window_width;
   valid_view[3] = ( output_point[2] < 0.0f );

   // corner 4
   corner_point[0] = face_xn;
   corner_point[1] = face_yp;
   corner_point[2] = face_zp;

   if( sides && (DRAW_BACK || DRAW_LEFT || DRAW_TOP) )
   {
      output_point_distance[4] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[4]     = output_point[0] / window_width;
   view_y[4]     = output_point[1] / window_width;
   valid_view[4] = ( output_point[2] < 0.0f );

   // corner 5
   corner_point[0] = face_xn;
   corner_point[1] = face_yp;
   corner_point[2] = face_zn;

   if( sides && (DRAW_BACK || DRAW_LEFT || DRAW_BOTTOM) )
   {
      output_point_distance[5] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[5]     = output_point[0] / window_width;
   view_y[5]     = output_point[1] / window_width;
   valid_view[5] = ( output_point[2] < 0.0f );

   // corner 6
   corner_point[0] = face_xn;
   corner_point[1] = face_yn;
   corner_point[2] = face_zp;

   if( sides && (DRAW_BACK || DRAW_RIGHT || DRAW_TOP) )
   {
      output_point_distance[6] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[6]     = output_point[0] / window_width;
   view_y[6]     = output_point[1] / window_width;
   valid_view[6] = ( output_point[2] < 0.0f );

   // corner 7
   corner_point[0] = face_xn;
   corner_point[1] = face_yn;
   corner_point[2] = face_zn;

   if( sides && (DRAW_BACK || DRAW_RIGHT || DRAW_BOTTOM) )
   {
      output_point_distance[7] = point_conversion( user_position,
                                                   user_direction,
                                                   window_distance,
                                                   corner_point,
                                                   rot,
                                                   output_point );
   }

   // scale to [-1, 1]
   view_x[7]     = output_point[0] / window_width;
   view_y[7]     = output_point[1] / window_width;
   valid_view[7] = ( output_point[2] < 0.0f );

   if ( sides & DRAW_FRONT )
   {
      if (valid_view[0] && valid_view[1] && valid_view[3] && valid_view[2])
      {
         // draw face x+
         vec1[0] = 0;
         vec1[1] = face_yp - face_yn;
         vec1[2] = 0;
      
         vec2[0] = 0;
         vec2[1] = 0;
         vec2[2] = face_zp - face_zn;
      
         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         linalg::unit_vector<float>  ( user_opposite, 3);
         brightness = linalg::dot_product<float>( normal, user_opposite, 3);
      
         vertices_x[0] = view_x[0];
         vertices_x[1] = view_x[1];
         vertices_x[2] = view_x[3];
         vertices_x[3] = view_x[2];
      
         vertices_y[0] = view_y[0];
         vertices_y[1] = view_y[1];
         vertices_y[2] = view_y[3];
         vertices_y[3] = view_y[2];
   
         distance[0] = output_point_distance[0];
         distance[1] = output_point_distance[1];
         distance[2] = output_point_distance[3];
         distance[3] = output_point_distance[2];
   
#ifdef ENABLE_RENDERING
         ogl::draw_polygon( vertices_x,
                            vertices_y,
                            distance,
                            color,
                            brightness,
                            4 );
#endif
      }
   }

   if ( sides & DRAW_BACK )
   {
      if (valid_view[4] && valid_view[5] && valid_view[7] && valid_view[6])
      {
         // draw face x-
         vec1[0] = 0;
         vec1[1] = 0;
         vec1[2] = face_zp - face_zn;
      
         vec2[0] = 0;
         vec2[1] = face_yp - face_yn;
         vec2[2] = 0;
      
         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         linalg::unit_vector<float>  ( user_opposite, 3);
         brightness = linalg::dot_product<float>( normal, user_opposite, 3);
      
         vertices_x[0] = view_x[4];
         vertices_x[1] = view_x[5];
         vertices_x[2] = view_x[7];
         vertices_x[3] = view_x[6];
      
         vertices_y[0] = view_y[4];
         vertices_y[1] = view_y[5];
         vertices_y[2] = view_y[7];
         vertices_y[3] = view_y[6];
   
         distance[0] = output_point_distance[4];
         distance[1] = output_point_distance[5];
         distance[2] = output_point_distance[7];
         distance[3] = output_point_distance[6];
   
#ifdef ENABLE_RENDERING
         ogl::draw_polygon( vertices_x,
                            vertices_y,
                            distance,
                            color,
                            brightness,
                            4 );
#endif
      }
   }

   if ( sides & DRAW_LEFT )
   {
      if (valid_view[0] && valid_view[1] && valid_view[5] && valid_view[4])
      {
         // draw face y+
         vec1[0] = 0;
         vec1[1] = 0;
         vec1[2] = face_zp - face_zn;
      
         vec2[0] = face_xp - face_xn;
         vec2[1] = 0;
         vec2[2] = 0;
      
         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         linalg::unit_vector<float>  ( user_opposite, 3);
         brightness = linalg::dot_product<float>( normal, user_opposite, 3);
      
         vertices_x[0] = view_x[0];
         vertices_x[1] = view_x[1];
         vertices_x[2] = view_x[5];
         vertices_x[3] = view_x[4];
      
         vertices_y[0] = view_y[0];
         vertices_y[1] = view_y[1];
         vertices_y[2] = view_y[5];
         vertices_y[3] = view_y[4];
   
         distance[0] = output_point_distance[0];
         distance[1] = output_point_distance[1];
         distance[2] = output_point_distance[5];
         distance[3] = output_point_distance[4];
   
#ifdef ENABLE_RENDERING
         ogl::draw_polygon( vertices_x,
                            vertices_y,
                            distance,
                            color,
                            brightness,
                            4 );
#endif
      }
   }

   if ( sides & DRAW_RIGHT )
   {
      if (valid_view[2] && valid_view[3] && valid_view[7] && valid_view[6])
      {
         // draw face y-
         vec1[0] = face_xp - face_xn;
         vec1[1] = 0;
         vec1[2] = 0;
      
         vec2[0] = 0;
         vec2[1] = 0;
         vec2[2] = face_zp - face_zn;
      
         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         linalg::unit_vector<float>  ( user_opposite, 3);
         brightness = linalg::dot_product<float>( normal, user_opposite, 3);
      
         vertices_x[0] = view_x[2];
         vertices_x[1] = view_x[3];
         vertices_x[2] = view_x[7];
         vertices_x[3] = view_x[6];
      
         vertices_y[0] = view_y[2];
         vertices_y[1] = view_y[3];
         vertices_y[2] = view_y[7];
         vertices_y[3] = view_y[6];
   
         distance[0] = output_point_distance[2];
         distance[1] = output_point_distance[3];
         distance[2] = output_point_distance[7];
         distance[3] = output_point_distance[6];
   
#ifdef ENABLE_RENDERING
         ogl::draw_polygon( vertices_x,
                            vertices_y,
                            distance,
                            color,
                            brightness,
                            4 );
#endif
      }
   }

   if ( sides & DRAW_TOP )
   {
      if (valid_view[0] && valid_view[2] && valid_view[6] && valid_view[4])
      {
         // draw face z+
         vec1[0] = face_xp - face_xn;
         vec1[1] = 0;
         vec1[2] = 0;
      
         vec2[0] = 0;
         vec2[1] = face_yp - face_yn;
         vec2[2] = 0;
      
         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         linalg::unit_vector<float>  ( user_opposite, 3);
         brightness = linalg::dot_product<float>( normal, user_opposite, 3);
      
         vertices_x[0] = view_x[0];
         vertices_x[1] = view_x[2];
         vertices_x[2] = view_x[6];
         vertices_x[3] = view_x[4];
      
         vertices_y[0] = view_y[0];
         vertices_y[1] = view_y[2];
         vertices_y[2] = view_y[6];
         vertices_y[3] = view_y[4];
   
         distance[0] = output_point_distance[0];
         distance[1] = output_point_distance[2];
         distance[2] = output_point_distance[6];
         distance[3] = output_point_distance[4];
   
#ifdef ENABLE_RENDERING
         ogl::draw_polygon( vertices_x,
                            vertices_y,
                            distance,
                            color,
                            brightness,
                            4 );
#endif
      }
   }

   if ( sides & DRAW_BOTTOM )
   {
      if (valid_view[1] && valid_view[3] && valid_view[7] && valid_view[5])
      {
         // draw face z-
         vec1[0] = 0;
         vec1[1] = face_yp - face_yn;
         vec1[2] = 0;
      
         vec2[0] = face_xp - face_xn;
         vec2[1] = 0;
         vec2[2] = 0;
      
         linalg::cross_product<float>( normal, vec2, vec1);
         linalg::unit_vector<float>  ( normal, 3);
         linalg::unit_vector<float>  ( user_opposite, 3);
         brightness = linalg::dot_product<float>( normal, user_opposite, 3);
      
         vertices_x[0] = view_x[1];
         vertices_x[1] = view_x[3];
         vertices_x[2] = view_x[7];
         vertices_x[3] = view_x[5];
      
         vertices_y[0] = view_y[1];
         vertices_y[1] = view_y[3];
         vertices_y[2] = view_y[7];
         vertices_y[3] = view_y[5];
   
         distance[0] = output_point_distance[1];
         distance[1] = output_point_distance[3];
         distance[2] = output_point_distance[7];
         distance[3] = output_point_distance[5];
   
#ifdef ENABLE_RENDERING
         ogl::draw_quads (
               vertices_x,
               vertices_y,
               distance,
               color,
               brightness );
#endif
      }
   }
}

/*
** function name: draw_block_2
*/
void draw_block_2 (
      float  block_position[3],
      float *user_position,
      float *user_direction,
      float  window_distance,
      float  window_width,
      int    sides,
      float *rot,
      float *color )
{
}
