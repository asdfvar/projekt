#ifndef MAP_H
#define MAP_H

#include "container.h"

class Cell { };

class MAP
{
   public:

      MAP (int size_in[3]);
      ~MAP (void);

      void update (void);

      const bool  *access_air    (void) { return (const bool*)air;     };
      const bool  *access_ground (void) { return (const bool*)ground;  };
      const float *access_weight (void) { return (const float*)weight; };

      void set_map_layer (int map_layer_in) { map_layer = map_layer_in; };
      bool test_los (int   cell[3]);
      bool test_los (float cell[3]);

      int  get_map_layer (void)             { return map_layer; };
      bool get_air_cell    (int ind);
      bool get_air_cell    (int ind[3]);
      bool get_ground_cell (int flattened_ind);
      bool get_ground_cell (int ind[3]);
      unsigned int get_material (int ind) { return material[ind]; };
      unsigned int get_material (int ind[3]);

      int shape (int dim) { return size[dim]; };

      void change (int flattened_cell_index, float value);
      void change (int cell[3], float value);
      void add_cell (unsigned int material_in, int flat_ind);
      void remove_cell (int flat_ind);

      void        ready_uncommited_job_cells (int cell_selections_in[2][3]);
      void        set_uncommited_job_cells (bool reset_uncommitted_jobs_size, int mode);
      void        unselect_uncommitted_jobs (void);
      const bool *access_uncommitted_jobs (void);

      unsigned int get_view_plain (int ind)    { return view_plain[ind]; };
      unsigned int get_view_plain (int ind[2]);

      void draw (float *transform, float *translation);

   private:

      float        *weight;
      bool         *air;
      bool         *ground;
      unsigned int *material;

      int map_layer;

      bool *uncommitted_jobs;

      unsigned int *view_plain;
      int *view_depth;
      int  max_depth;

      Lattice<Cell> *cells;

      int size[3];

      void set_map (void);
      void set_ground (void);

      void local_change (int flattened_cell_index, float value);

      int cell_selections[2][3];
};

#endif
