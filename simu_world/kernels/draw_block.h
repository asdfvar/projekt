#ifndef DRAW_BLOCK_H
#define DRAW_BLOCK_H

#include "user.h"
#include "opengl_interface.h"

void draw_block( float  block_position[3],
                 float *color,
                 float *vertices,
                 User  *user);

#endif
