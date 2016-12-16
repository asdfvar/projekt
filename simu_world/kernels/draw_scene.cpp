#include "draw_scene.h"
#include "draw_block.h"
#include "user.h"
#include "map.h"
#include "hud.h"
#include "point_conversion.h"
#include "linalg.h"
#include <cmath>

/*
** function: draw_scene
**
** The coordinates are converted so that they are relative
** to the user.
*/
void draw_scene( User                  *user,
                 ogl::Opengl_interface *opengl_interface,
                 Map                   *map)
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
                  opengl_interface,
                  user);
   }

   hud::display();

}
