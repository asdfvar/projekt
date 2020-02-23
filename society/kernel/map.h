#ifndef MAP_H
#define MAP_H

#include "container.h"

class Cell
{

};

class MAP
{
   public:

      MAP (int size_in[3]);
      ~MAP (void);

      void update (void);

      const float *access_map         (void) { return (const float*)map;    };
      const float *access_ground      (void) { return (const float*)ground; };

      void set_map_layer (int map_layer_in) { map_layer = map_layer_in; };

      float get_ground_cell (int flattened_ind);
      float get_ground_cell (int ind[3]);

      int map_dim (int dim) { return size[dim]; };

      void change (int flattened_cell_index, float value);
      void change (int cell[3], float value);

      int  get_material (int ind) { return material[ind]; };

      void       ready_jobs (int cell_selections[2][3], bool control_down);
      void       unselect_uncommitted_jobs (void);
      const int *access_uncommitted_jobs (int *size);

      void draw_map (float *transform, float *translation);

   private:

      float *map;
      float *ground;
      int   *material;

      int map_layer;

      int *uncommitted_jobs;
      int  uncommitted_jobs_size;

      int *view_plain;

      Lattice<Cell> *cells;

      int size[3];

      void set_ground (void);

      void local_change (int flattened_cell_index, float value);
};

#endif
