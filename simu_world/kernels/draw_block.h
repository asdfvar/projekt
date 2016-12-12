#ifndef DRAW_BLOCK_H
#define DRAW_BLOCK_H

#include "user.h"
#include "opengl_interface.h"

void draw_block( float                  block_position[3],
                 ogl::Opengl_interface *opengl_interface,
                 User                  *user);

#endif
