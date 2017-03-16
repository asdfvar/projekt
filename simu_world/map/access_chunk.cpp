#include "map.h"

/*
** function name: access_chunk from: Map
** Return pointer to the specified chunk block from physical id input
*/
Chunk *Map::access_chunk( int p_id_x,
                          int p_id_y,
                          int p_id_z )
{
   if ((p_id_x < 0) || (p_id_x >= num_chunks[0]) ||
       (p_id_y < 0) || (p_id_y >= num_chunks[1]) ||
       (p_id_z < 0) || (p_id_z >= num_chunks[2]))
   {
      std::cout << "select index of " << p_id_x << ", "  << p_id_y << ", "  << p_id_z <<
                   " out of bounds: " << num_chunks[0] << ", " <<
                                          num_chunks[1] << ", " <<
                                         num_chunks[2] << std::endl;
   }

   unsigned int index =  p_id_z * num_chunks[0] * num_chunks[1] +
                         p_id_y * num_chunks[0] +
                         p_id_x;

   return chunks->at( index );
}
