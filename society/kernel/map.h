#ifndef MAP_H
#define MAP_H

#include <list>

class MAP
{
   public:

      MAP (int num_cells[3]);
      ~MAP (void);

      const float *access_map     (void) { return (const float*)map;          };
      const float *access_ground  (void) { return (const float*)ground;       };
      const int   *access_actions (void) { return (const int*  )action_cells; };

      int map_dim (int dim) { return size[dim]; };

      void change (int flattened_cell_index, float value);
      void change (int cell[3], float value);

      void set_dig (float selection_box[2][3]);

   private:

      float *map;
      float *ground;
      int   *action_cells;

      int size[3];

      void set_ground (void);

      void local_change (int flattened_cell_index, float value);
};

#endif
