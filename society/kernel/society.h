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

      void set_dig_actions (void);

      // Graphics
      void draw_units (float *transform, float *translation, int map_layer);

      void draw_uncommitted_actions (
            float       *transform,
            float       *translation,
            float        color[3],
            int          map_layer);

      void draw_committed_actions (
            float       *transform,
            float       *translation,
            float        color[3],
            int          map_layer);

      void draw_assigned_actions (
            float       *transform,
            float       *translation,
            float        color[3],
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

      std::vector<Unit*> units;
      std::list<Action*> committed_actions;
      std::list<Action*> uncommitted_actions;
      std::list<Action*> assigned_actions;

      float window_to_cell (float point, int dim);
};

#endif
