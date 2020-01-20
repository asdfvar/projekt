#ifndef SOCIETY_H
#define SOCIETY_H

#include "map.h"
#include <queue>
#include <vector>
#include <list>
#include "unit.h"
#include "actions.h"
#include "container.h"

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

      void set_group (int group_number);

      void select_group (int group_number);

      void select_cells (int cell_selections[2][3], bool control_down);

      int get_unit_info (float *x, float *y, float *z, bool *selections);

      void set_actions (int action_type);

      // Graphics
      void draw_units (float *transform, float *translation, int map_layer);

      void draw_uncommitted_actions (
            float       *transform,
            float       *translation,
            int          map_layer);

      void draw_committed_actions (
            float       *transform,
            float       *translation,
            int          map_layer);

      void draw_assigned_actions (
            float       *transform,
            float       *translation,
            int          map_layer);

      void draw_unit_actions (
            float       *transform,
            float       *translation,
            int          map_layer);

   private:

      MAP *Map;
      int  dim[3];

      float accum_time;
      float accum_time_limit;
      bool  rolled;


      float *cost;
      float *buffer;

      int *scratch;

      Container<Unit>   units;
      Container<Action> committed_actions;
      Container<Action> actions;

      Container<Unit>   group[10];

      float window_to_cell (float point, int dim);
};

#endif
