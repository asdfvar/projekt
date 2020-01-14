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

   private:

      float *map;
      float *ground;
      int   *material;

      int size[3];

      void set_ground (void);

      void local_change (int flattened_cell_index, float value);
};

#endif
