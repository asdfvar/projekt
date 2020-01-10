#ifndef SOCIETY_H
#define SOCIETY_H

#include "map.h"
#include <queue>
#include <vector>
#include <list>
#include "unit.h"
#include "actions.h"

class Society
{
   public:

      Society (void);
      ~Society (void);

      void update (float time_step);

      void set_destination (int dest[3]);

      const float *access_map ();

      const int get_size (int ind) { return dim[ind]; };

      void select_units (int cell_selections[2][3], int map_layer, bool control_down);

      void select_all (void);

      void unselect_all (void);

      void select_cells (int cell_selections[2][3], bool control_down);

      int get_unit_info (float *x, float *y, float *z, bool *selections);

      const int *access_uncommitted_dig_actions (int *size);
      const int *access_dig_actions             (int *size);
      void       set_dig_actions                (void     );

   private:

      MAP *Map;
      int  dim[3];

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
