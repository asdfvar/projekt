#ifndef MAP_H
#define MAP_H

#include <list>

class MAP
{
   public:

      MAP (int num_cells[3]);
      ~MAP (void);

      const float *access_map         (void) { return (const float*)map;          };
      const float *access_ground      (void) { return (const float*)ground;       };

      int map_dim (int dim) { return size[dim]; };

      void change (int flattened_cell_index, float value);
      void change (int cell[3], float value);

      int  get_material (int ind) { return material[ind]; };

      void       ready_actions (int cell_selections[2][3], bool control_down);
      void       unselect_uncommitted_actions (void);
      const int *access_uncommitted_actions (int *size);

   private:

      float *map;
      float *ground;
      int   *material;

      int *uncommitted_actions;
      int  uncommitted_actions_size;

      int size[3];

      void set_ground (void);

      void local_change (int flattened_cell_index, float value);
};

#endif
