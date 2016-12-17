#include "draw_scene.h"
#include "draw_block.h"
#include "user.h"
#include "map.h"
#include "hud.h"
#include "point_conversion.h"
#include <cmath>

/*
** function: draw_scene
**
** The coordinates are converted so that they are relative
** to the user.
*/
void draw_scene( User                  *user,
                 ogl::Opengl_interface *opengl_interface,
                 Map_grid              *map_grid,
                 Map                   *map)
{

   float block_position[3];

   map = map_grid->access_map( 5, 5, 4);

   // iterate through all the blocks
   for (int block_ind = 0; block_ind < map->get_dimensions(); block_ind++)
   {

      if (!map->get_position( block_position, block_ind)) continue;

      draw_block( block_position,
                  opengl_interface,
                  user);
   }

   hud::display();

}
