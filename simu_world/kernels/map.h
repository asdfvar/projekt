#ifndef MAP_H
#define MAP_H

#include "chunk.h"
#include <iostream>
#include <vector>
#include "opengl_interface.h"
#include "user.h"

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

      int get_grid_size(int ind);

   private:
      void shift( int x, int y, int z);

      int local_grid_size[3];

      int *virtual_grid_id_x,
          *virtual_grid_id_y,
          *virtual_grid_id_z;

      std::vector<Chunk*> chunks;

};

#endif
