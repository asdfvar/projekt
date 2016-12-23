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
                 Map_grid              *map_grid)
{

   map_grid->render_map( opengl_interface, user);

   hud::display();

}
