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
void draw_scene( User *user, Map_grid *map_grid)
{

   map_grid->render_chunk( user);

   hud::display();

}
