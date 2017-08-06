#include <cmath>
#include "map.h"
#include "draw_block.h"
#include "point_conversion.h"

/*
** function name: render_chunk from: Map
*/
void Map::render_chunk( float* user_position,
                        float* user_direction,
                        float  window_distance,
                        float  window_width )
{
   float block_position[3];

   float rot[9];
   rotation( user_direction,
             rot );

   int total_dim = dim_x * dim_y * dim_z;

   // iterate through all the blocks
   for (int block_ind = 0, block_ind_z = 0; block_ind_z < dim_z; block_ind_z++)
   {
      for (int block_ind_y = 0; block_ind_y < dim_y; block_ind_y++)
      {
         for (int block_ind_x = 0; block_ind_x < dim_x; block_ind_x++, block_ind++)
         {

            float color[3] = { 1.0f, 1.0f, 1.0f };

            int block = blocks[ block_ind ];
            if ( block == 0 ) continue;

            if ( block == 1 )
            {
               color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f;
            }
            else if ( block == 2 )
            {
               color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
            }
            else if ( block == 3 )
            {
               color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
            }
            else if ( block == 4 )
            {
               color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
            }

            // absolute block position
            block_position[0] = (float)( block_ind_x -
                                         num_chunks_x / 2 *
                                         chunk_dim_x ) +
                                         map_pos_x;

            block_position[1] = (float)( block_ind_y -
                                         num_chunks_y / 2 *
                                         chunk_dim_y ) +
                                         map_pos_y;

            block_position[2] = (float)( block_ind_z -
                                         num_chunks_z / 2 *
                                         chunk_dim_z ) +
                                         map_pos_z;

            int sides = 0;

            if (blocks[ std::min(block_ind + 1, total_dim) ] <= 0)
            {
               sides |= DRAW_FRONT;
            }
            if (blocks[ std::max(block_ind - 1, 0) ] <= 0)
            {
               sides |= DRAW_BACK;
            }
            if (blocks[ std::min(block_ind + dim_x, total_dim) ] <= 0)
            {
               sides |= DRAW_LEFT;
            }
            if (blocks[ std::max(block_ind - dim_x, 0) ] <= 0)
            {
               sides |= DRAW_RIGHT;
            }
            if (blocks[ std::min(block_ind + dim_x * dim_y, total_dim) ] <= 0)
            {
               sides |= DRAW_TOP;
            }
            if (blocks[ std::max(block_ind - dim_x * dim_y, 0) ] <= 0)
            {
               sides |= DRAW_BOTTOM;
            }

            if (block_position[0] > user_position[0])
            {
               sides &= ~DRAW_FRONT;
            }
            if (block_position[1] > user_position[1])
            {
               sides &= ~DRAW_LEFT;
            }
            if (block_position[2] > user_position[2])
            {
               sides &= ~DRAW_TOP;
            }
            if (block_position[0] < user_position[0])
            {
               sides &= ~DRAW_BACK;
            }
            if (block_position[1] < user_position[1])
            {
               sides &= ~DRAW_RIGHT;
            }
            if (block_position[2] < user_position[2])
            {
               sides &= ~DRAW_BOTTOM;
            }

            draw_block( block_position,
                        user_position,
                        user_direction,
                        window_distance,
                        window_width,
                        sides,
                        rot,
                        color );
         }
      }
   }
}
