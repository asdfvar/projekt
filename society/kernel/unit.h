#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include "map.h"

class Unit {

   public:

      Unit (float position_x,
            float position_y,
            float position_z,
            MAP*);

     ~Unit (void);

      float get_position (int ind) { return position[ind]; };

      float get_speed (void) { return speed; };

      void get_destination (int *dest_out);

      void set_destination (int dest_in[3]);

      void unselect (void) { selected = false; };
      void select (void)   { selected = true;  };

      bool is_selected (void) { return selected; };

      void update (
            std::vector<Unit*> &all_units,
            float time_step);

   private:

      MAP   *Map;
      int   *path;

      float *cost;
      float *buffer;
      int *scratch;

      float position[3];

      int   dest[3];

      float max_speed;
      float speed;
      float direction;

      int  next_cell[3];
      bool selected;

      int path_size;
};

#endif
