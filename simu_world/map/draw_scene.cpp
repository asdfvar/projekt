#include "draw_scene.h"
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

/*
** function: draw_scene
**
** The coordinates are converted so that they are relative
** to the user.
*/
void draw_scene( User *user, Map  *map)
{

int number_of_blocks;
//<<<
number_of_blocks = map->get_dimensions();
//>>>

   float block_position[3];

   // iterate through all the blocks
   for (int block_ind = 0; block_ind < number_of_blocks; block_ind++)
   {

      if (!map->get_position( block_position, block_ind)) continue;

      draw_block( block_position,
                  user);



   }

//<<<
block_position[0] = 4.0f;
block_position[1] = 0.0f;
block_position[2] = 0.0f;
//>>>

   draw_block( block_position,
               user);

}
