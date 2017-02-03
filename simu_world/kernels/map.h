#ifndef MAP_H
#define MAP_H

#include "chunk.h"
#include <iostream>
#include <vector>
#include "opengl_interface.h"
#include "user.h"
#include "text.h"

/*
** class name: Map
*/
class Map
{
   public:
      Map(void);
     ~Map(void);

      void update( void );
      void update( float *position);
      void render_chunk( User *user);

      Chunk *access_chunk(int v_id_x,
                      int v_id_y,
                      int v_id_z);

      void debug_info( void );
      int  get_abs_element( int *position, Text *text);

   private:
      void shift( int x, int y, int z);

      int num_chunks[3];
      int num_chunk_elements[3];

      int *physical_chunk_id_x,
          *physical_chunk_id_y,
          *physical_chunk_id_z;

      int *virtual_chunk_id_x,
          *virtual_chunk_id_y,
          *virtual_chunk_id_z;

      std::vector<Chunk*> chunks;

      float *vertices;

};

#endif
