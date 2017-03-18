#ifndef DRAW_BLOCK_H
#define DRAW_BLOCK_H

#define DRAW_FRONT  0x01
#define DRAW_BACK   0x02
#define DRAW_LEFT   0x04
#define DRAW_RIGHT  0x08
#define DRAW_TOP    0x10
#define DRAW_BOTTOM 0x20

void draw_block( float  block_position[3],
                 float *user_position,
                 float *user_direction,
                 float  window_distance,
                 float  window_width,
                 int    side,
                 float *rot,
                 float *color );

#endif
