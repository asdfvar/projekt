#include "draw_scene.h"
#include "draw_block.h"
#include "user.h"
#include "map.h"
#include "hud.h"
#include "point_conversion.h"
#include <cmath>
#include "text.h"

/*
** function: draw_scene
**
** The coordinates are converted so that they are relative
** to the user.
*/
void draw_scene( User *user, Map *map)
{

   map->render_chunk( user);

   hud::display();

//   txt::write_to_screen( "A letter", -0.5f, +0.5f);
}
