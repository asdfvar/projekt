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

   for (int grid_x = 0; grid_x < map_grid->get_grid_size(0); grid_x++)
   {
      for (int grid_y = 0; grid_y < map_grid->get_grid_size(1); grid_y++)
      {
         for (int grid_z = 0; grid_z < map_grid->get_grid_size(2); grid_z++)
         {
            map = map_grid->access_map( grid_x, grid_y, grid_z);

            // iterate through all the blocks
            for (unsigned int block_ind = 0; block_ind < map->get_dimensions(); block_ind++)
            {

               if (!map->get_position( block_position, block_ind)) continue;

               draw_block( block_position,
                           opengl_interface,
                           user);
         }
      }
   }
   }

   hud::display();

}
