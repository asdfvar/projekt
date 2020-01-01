#ifndef SOCIETY_H
#define SOCIETY_H

#include "map.h"
#include <queue>
#include <vector>
#include <list>
#include "unit.h"

class Action
{
   int type;
};

class Society
{
   public:

      Society (void);
      ~Society (void);

      void update (float time_step);

      void set_destination (int dest[3]);

      const float *access_map (int *dim_x, int *dim_y, int *dim_z);

      void select_units (float selection_box[2][3], int map_layer, bool control_down);

      void select_all (void);

      void unselect_all (void);

      void select_cells (int cell_selections[2][3]);

      int get_unit_info (float *x, float *y, float *z, bool *selections);

      int get_actions (int action, int map_layer, int *actions);

   private:

      MAP *Map;
      int  dim_x; // TODO: change these to: dim[3]
      int  dim_y;
      int  dim_z;

      float accum_time;
      float accum_time_limit;
      bool  rolled;


      float *cost;
      float *buffer;

      int *scratch;

      std::vector<Unit*> units;
      std::list<Action*> actions;

      float window_to_cell (float point, int dim);
};

#endif
