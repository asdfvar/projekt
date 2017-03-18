#include "map.h"
#include "draw_block.h"
#include "point_conversion.h"

/*
** function name: render_chunk from: Map
*/
void Map::render_chunk( User *user)
{
   float block_position[3];

   int mid_chunk[3] = { num_chunks[0] / 2,
                        num_chunks[1] / 2,
                        num_chunks[2] / 2 };

   float user_position[3];
   user->get_position( user_position );

   float user_direction[3];
   user->get_direction( user_direction );

   float rot[9];
   rotation( user_direction,
             rot );

   float window_distance = user->get_window_distance();
   float window_width    = user->get_window_width();

   // set the current chunk to the base chunk
   chunks->set_base();

   int num_chunk_elements[3];
   Chunk *chunk = access_chunk( 0, 0, 0 );
   num_chunk_elements[0] = chunk->get_dimension( 0 );
   num_chunk_elements[1] = chunk->get_dimension( 1 );
   num_chunk_elements[2] = chunk->get_dimension( 2 );

   // iterate through all the chunks
   for (int chunk_ind_z = 0; chunk_ind_z < num_chunks[2]; chunk_ind_z++)
   {
      for (int chunk_ind_y = 0; chunk_ind_y < num_chunks[1]; chunk_ind_y++)
      {
         for (int chunk_ind_x = 0; chunk_ind_x < num_chunks[0]; chunk_ind_x++)
         {
            chunk = access_chunk( chunk_ind_x, chunk_ind_y, chunk_ind_z);

            int chunk_sides = 0;

            if ( virtual_chunk_id_x[ chunk_ind_x ] >= mid_chunk[0] )
            {
               chunk_sides |= DRAW_BACK;
            }
            if ( virtual_chunk_id_x[ chunk_ind_x ] <= mid_chunk[0] )
            {
               chunk_sides |= DRAW_FRONT;
            }
            if ( virtual_chunk_id_y[ chunk_ind_y ] >= mid_chunk[1] )
            {
               chunk_sides |= DRAW_RIGHT;
            }
            if ( virtual_chunk_id_y[ chunk_ind_y ] <= mid_chunk[1] )
            {
               chunk_sides |= DRAW_LEFT;
            }
            if ( virtual_chunk_id_z[ chunk_ind_z ] >= mid_chunk[2] )
            {
               chunk_sides |= DRAW_BOTTOM;
            }
            if ( virtual_chunk_id_z[ chunk_ind_z ] <= mid_chunk[2] )
            {
               chunk_sides |= DRAW_TOP;
            }

            if ( chunk->is_valid() )
            {
               // iterate through all the blocks
               for (int block_ind = 0, block_ind_z = 0; block_ind_z < num_chunk_elements[2]; block_ind_z++)
               {
                  for (int block_ind_y = 0; block_ind_y < num_chunk_elements[1]; block_ind_y++)
                  {
                     for (int block_ind_x = 0; block_ind_x < num_chunk_elements[0]; block_ind_x++, block_ind++)
                     {

                        int block_sides = 0;
      
                        int block = chunk->get_position( block_position, block_ind );
                        if ( block == 0 ) continue;
      
                        /*
                        ** draw each face of the block that doesn't have a neighboring block
                        */
                        // front
                        int front_block = chunks->get_block( block_ind_x + 1,
                                                             block_ind_y,
                                                             block_ind_z );
                        if (front_block == 0) block_sides |= DRAW_FRONT;
      
                        // back
                        int back_block = chunks->get_block( block_ind_x - 1,
                                                            block_ind_y,
                                                            block_ind_z );
                        if (back_block == 0) block_sides |= DRAW_BACK;
      
                        // left
                        int left_block = chunks->get_block( block_ind_x,
                                                            block_ind_y + 1,
                                                            block_ind_z );
                        if (left_block == 0) block_sides |= DRAW_LEFT;

                        // right
                        int right_block = chunks->get_block( block_ind_x,
                                                             block_ind_y - 1,
                                                             block_ind_z );
                        if (right_block == 0) block_sides |= DRAW_RIGHT;
      
                        // top
                        int top_block = chunks->get_block( block_ind_x,
                                                           block_ind_y,
                                                           block_ind_z + 1 );
                        if (top_block == 0) block_sides |= DRAW_TOP;
      
                        // bottom
                        int bottom_block = chunks->get_block( block_ind_x,
                                                              block_ind_y,
                                                              block_ind_z - 1 );
                        if (bottom_block == 0) block_sides |= DRAW_BOTTOM;
      
                        float color[3];
                        chunk->get_color( color );
      
                        int sides = chunk_sides & block_sides;
      
                        if ( sides )
                        {
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
            }

            chunks->next();

         }
      }
   }
}
