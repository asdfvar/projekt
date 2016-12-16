#ifndef DRAW_SCENE_H
#define DRAW_SCENE_H

#include "user.h"
#include "map.h"
#include "opengl_interface.h"

void draw_scene( User                  *user,
                 ogl::Opengl_interface *opengl_interface,
                 Map_grid              *map_grid,
                 Map                   *map);

#endif
