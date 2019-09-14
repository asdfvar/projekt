#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>

void draw_map (const float *map, int map_dims[3], int map_layer);

#endif
