#include "change_direction.h"
#include <iostream>
#include <cmath>

/*
** function name: change_direction
**
** change the user's look direction based on the x and y offsets on the
** view window
**
** the direction is assumed to be of unit length on input
*/
void change_direction( float*      direction,
                       float       window_distance,
                       float       x_offset,
                       float       y_offset,
                       const float sensitivity )
{

         float norm_xy = sqrtf( direction[0] * direction[0] +
                                direction[1] * direction[1] );
   
         /*
         ** given an x-offset on the window, sin and cos of the
         ** angle offsets are computed directly. +x is a negative
         ** angle offset. first-order angle approximations are used
         ** to save on processing resources.
         **
         **            ---> +x w.r.t. observer
         **    x_offset
         ** --------------------
         **    \       |
         **     \      |
         **      \     |
         **       \    | window_distance
         **        \   |
         **         \  |
         **          \ |
         **           \|
         **            |
         */

         x_offset *= sensitivity;

         float sin_x = -x_offset / window_distance;
         // cos_x = window_distance / ~window_distance ~= 1;

         float temp[3];

         /*
         ** apply a rotation about the z-axis
         */
         temp[0] =         direction[0] - sin_x * direction[1];
         temp[1] = sin_x * direction[0] +         direction[1];
      
         direction[0] = temp[0];
         direction[1] = temp[1];
      
         /*
         ** given the norm( direction ) = 1 before, norm( direction ) ~= 1
         ** now due to the small angle approximation assumption
         */
   
         y_offset *= sensitivity;

         float sin_y = y_offset / window_distance;
         // cos_y = window_distance / ~window_distance ~= 1;
      
         /*
         ** to change the pitch of the direction, a rotation about
         ** the z-axis to put the direction
         ** onto the y-axis is done first followed by a rotation
         ** about the x-axis (change in pitch) followed by a rotation
         ** back about the z-axis
         */

         /*
         ** rot_z = [[  sin_phi, -cos_phi,  0  ]
         **          [  cos_phi,  sin_phi,  0  ]
         **          [        0,        0,  1  ]]
         */
         float rot_z[3][3];
         rot_z[0][0] =  direction[1] / norm_xy;
         rot_z[0][1] = -direction[0] / norm_xy;
         rot_z[0][2] =  0.0f;
   
         rot_z[1][0] =  direction[0] / norm_xy;
         rot_z[1][1] =  direction[1] / norm_xy;
         rot_z[1][2] =  0.0f;
   
         rot_z[2][0] =  0.0f;
         rot_z[2][1] =  0.0f;
         rot_z[2][2] =  1.0f;
   
         /*
         ** rot_x = [[  1,  0,      0      ]
         **          [  0,  cos_y, -sin_y  ]
         **          [  0,  sin_y,  cos_y  ]]
         */
         float rot_x[3][3];
         rot_x[0][0] = 1.0f; rot_x[0][1] = 0.0f;  rot_x[0][2] =  0.0f;
         rot_x[1][0] = 0.0f; rot_x[1][1] = 1.0f;  rot_x[1][2] = -sin_y;
         rot_x[2][0] = 0.0f; rot_x[2][1] = sin_y; rot_x[2][2] =  1.0f;
   
         /*
         ** rot_z_back = [[  sin_phi,  cos_phi,  0  ]
         **               [ -cos_phi,  sin_phi,  0  ]
         **               [        0,        0,  1  ]]
         */
         float rot_z_back[3][3];
         rot_z_back[0][0] = direction[1] / norm_xy;
         rot_z_back[0][1] = direction[0] / norm_xy;
         rot_z_back[0][2] = 0.0f;
   
         rot_z_back[1][0] = -direction[0] / norm_xy;
         rot_z_back[1][1] =  direction[1] / norm_xy;
         rot_z_back[1][2] =  0.0f;
   
         rot_z_back[2][0] = 0.0f;
         rot_z_back[2][1] = 0.0f;
         rot_z_back[2][2] = 1.0f;
   
         /*
         ** new direction = rot_z_back * rot_x * rot_z * direction
         */
         temp[0] = direction[0] * rot_z[0][0] +
                   direction[1] * rot_z[0][1] +
                   direction[2] * rot_z[0][2];
   
         temp[1] = direction[0] * rot_z[1][0] +
                   direction[1] * rot_z[1][1] +
                   direction[2] * rot_z[1][2];
   
         temp[2] = direction[0] * rot_z[2][0] +
                   direction[1] * rot_z[2][1] +
                   direction[2] * rot_z[2][2];
   
         direction[0] = temp[0];
         direction[1] = temp[1];
         direction[2] = temp[2];
   
         temp[0] = direction[0] * rot_x[0][0] +
                   direction[1] * rot_x[0][1] +
                   direction[2] * rot_x[0][2];
   
         temp[1] = direction[0] * rot_x[1][0] +
                   direction[1] * rot_x[1][1] +
                   direction[2] * rot_x[1][2];
   
         temp[2] = direction[0] * rot_x[2][0] +
                   direction[1] * rot_x[2][1] +
                   direction[2] * rot_x[2][2];
   
         direction[0] = temp[0];
         direction[1] = temp[1];
         direction[2] = temp[2];
   
         temp[0] = direction[0] * rot_z_back[0][0] +
                   direction[1] * rot_z_back[0][1] +
                   direction[2] * rot_z_back[0][2];
   
         temp[1] = direction[0] * rot_z_back[1][0] +
                   direction[1] * rot_z_back[1][1] +
                   direction[2] * rot_z_back[1][2];
   
         temp[2] = direction[0] * rot_z_back[2][0] +
                   direction[1] * rot_z_back[2][1] +
                   direction[2] * rot_z_back[2][2];
   
         direction[0] = temp[0];
         direction[1] = temp[1];
         direction[2] = temp[2];
   
         /*
         ** renormalise to enforce the unit length of the direction.
         ** this is necessary because any perturbation will
         ** aggregate over the iterations otherwise.
         */
         float norm = sqrtf( direction[0] * direction[0] +
                             direction[1] * direction[1] +
                             direction[2] * direction[2] );

         float norm_inv = 1.0f / norm;
   
         direction[0] *= norm_inv;
         direction[1] *= norm_inv;
         direction[2] *= norm_inv;
}
