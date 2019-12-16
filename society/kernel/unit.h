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

      float get_position_x (void) { return position_x; };
      float get_position_y (void) { return position_y; };
      float get_position_z (void) { return position_z; };

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
      float *cost;
      float *buffer;
      int *scratch;

      float position_x;
      float position_y;
      float position_z;

      int   dest[3];

      float max_speed;
      float speed;
      float direction;

      bool update_path;
      int  next_cell[3];
      bool selected;
};

#endif
