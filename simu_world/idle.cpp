#include "opengl_interface.h"
#include <iostream>

void Opengl_interface::idle( void)
{

   /*
   ** wait for the prescribed time to pass
   */
   time_manager->wait_for_time();

   user.update( time_manager->get_time_step() );

   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   if (first_frame)
   {
      glutWarpPointer( window_center_x, window_center_y); // Moves the curser to this position in pixels
      mousePassivePosition[0] = window_center_x;
      mousePassivePosition[1] = window_center_y;
      first_frame = false;
   }
   else if( mouse_passive_done)
   {
      int x_offset = mousePassivePosition[0] - window_center_x;
      int y_offset = window_center_y - mousePassivePosition[1];
   
      const float toggle = 0.003f;
      float cos_x = cosf( -(float)x_offset * toggle);
      float sin_x = sinf( -(float)x_offset * toggle);
      float cos_y = cosf(  (float)y_offset * toggle);
      float sin_y = sinf(  (float)y_offset * toggle);
   
      float direction[3];
      user.get_direction( direction);
   
      float temp[3];
      temp[0] = cos_x * direction[0] - sin_x * direction[1];
      temp[1] = sin_x * direction[0] + cos_x * direction[1];
   
      direction[0] = temp[0];
      direction[1] = temp[1];
   
      float norm = sqrtf( direction[0] * direction[0] +
                          direction[1] * direction[1] +
                          direction[2] * direction[2]);

      direction[0] /= norm;
      direction[1] /= norm;
      direction[2] /= norm;

      float norm_xy = sqrtf( direction[0] * direction[0] +
                             direction[1] * direction[1]);

      float rot_z[3][3];
      rot_z[0][0] = direction[1] / norm_xy; rot_z[0][1] = -direction[0] / norm_xy; rot_z[0][2] = 0.0f;
      rot_z[1][0] = direction[0] / norm_xy; rot_z[1][1] =  direction[1] / norm_xy; rot_z[1][2] = 0.0f;
      rot_z[2][0] = 0.0f;                   rot_z[2][1] =  0.0f;                   rot_z[2][2] = 1.0f;

      float rot_x[3][3];
      rot_x[0][0] = 1.0f; rot_x[0][1] =  0.0f; rot_x[0][2] =  0.0f;
      rot_x[1][0] = 0.0f; rot_x[1][1] = cos_y; rot_x[1][2] = -sin_y;
      rot_x[2][0] = 0.0f; rot_x[2][1] = sin_y; rot_x[2][2] =  cos_y;

      float rot_z_back[3][3];
      rot_z_back[0][0] = direction[1] / norm_xy; rot_z_back[0][1]
                       = direction[0] / norm_xy; rot_z_back[0][2]
                       = 0.0f;
      rot_z_back[1][0] = -direction[0] / norm_xy; rot_z_back[1][1]
                       = direction[1] / norm_xy; rot_z_back[1][2]
                       = 0.0f;
      rot_z_back[2][0] = 0.0f;
      rot_z_back[2][1] = 0.0f;
      rot_z_back[2][2] = 1.0f;

      temp[0] = direction[0] * rot_z[0][0] + direction[1] * rot_z[0][1] + direction[2] * rot_z[0][2];
      temp[1] = direction[0] * rot_z[1][0] + direction[1] * rot_z[1][1] + direction[2] * rot_z[1][2];
      temp[2] = direction[0] * rot_z[2][0] + direction[1] * rot_z[2][1] + direction[2] * rot_z[2][2];

      direction[0] = temp[0];
      direction[1] = temp[1];
      direction[2] = temp[2];

      temp[0] = direction[0] * rot_x[0][0] + direction[1] * rot_x[0][1] + direction[2] * rot_x[0][2];
      temp[1] = direction[0] * rot_x[1][0] + direction[1] * rot_x[1][1] + direction[2] * rot_x[1][2];
      temp[2] = direction[0] * rot_x[2][0] + direction[1] * rot_x[2][1] + direction[2] * rot_x[2][2];

      direction[0] = temp[0];
      direction[1] = temp[1];
      direction[2] = temp[2];

      temp[0] = direction[0] * rot_z_back[0][0] + direction[1] * rot_z_back[0][1] + direction[2] * rot_z_back[0][2];
      temp[1] = direction[0] * rot_z_back[1][0] + direction[1] * rot_z_back[1][1] + direction[2] * rot_z_back[1][2];
      temp[2] = direction[0] * rot_z_back[2][0] + direction[1] * rot_z_back[2][1] + direction[2] * rot_z_back[2][2];

      direction[0] = temp[0];
      direction[1] = temp[1];
      direction[2] = temp[2];

      norm = sqrtf( direction[0] * direction[0] +
                    direction[1] * direction[1] +
                    direction[2] * direction[2]);

      direction[0] /= norm;
      direction[1] /= norm;
      direction[2] /= norm;

      user.set_direction( direction);
   
      glutWarpPointer( window_center_x, window_center_y);

      mouse_passive_done = false;
   }

}
