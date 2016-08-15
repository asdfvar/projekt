#include <cmath>
#include "user.h"
#include "linalg.h"

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
      map->get_position( block_position, block_ind);

      // get position of block relative to user but at absolute direction
      float user_to_block[3];
      user_to_block[0] = block_position[0] - position[0];
      user_to_block[1] = block_position[1] - position[1];
      user_to_block[2] = block_position[2] - position[2];

      // rotate to user perspective to get relative orientation
      /*
      ** [ cos(hor_angle) -sin(hor_angle) 0 ]
      ** [ sin(hor_angle)  cos(hor_angle) 0 ]
      ** [ 0               0              1 ]
      */
      float xp = user_to_block[0]*cosf(hor_angle) - user_to_block[1]*sinf(hor_angle);
      float yp = user_to_block[0]*sinf(hor_angle) + user_to_block[1]*cosf(hor_angle);
      float zp = user_to_block[2];

      /*
      ** [ cos(hor_angle) 0 -sin(hor_angle) ]
      ** [ 0              1  0              ]
      ** [ sin(hor_angle) 0  cos(hor_angle) ]
      */
      user_to_block[0] = xp*cosf(vert_angle) - zp*sinf(vert_angle);
      user_to_block[1] = yp;
      user_to_block[2] = xp*sinf(vert_angle) + zp*cosf(vert_angle);

      // check that each point falls withing the viewing window
//      linalg::cross_product<float>(c, a, b); // c = a x b
   }
   
}
