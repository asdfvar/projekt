#ifndef UNIT_H
#define UNIT_H

#include "map.h"

class Unit {

   public:

      Unit (float position_x, float position_y, float position_z, MAP*, float *scratch);
     ~Unit (void);

      float get_position_x (void) { return position_x; };
      float get_position_y (void) { return position_y; };
      float get_position_z (void) { return position_z; };

      float get_speed (void) { return speed; };

      void set_destination (int dest_in[3]) {
         dest[0] = dest_in[0],
         dest[1] = dest_in[1],
         dest[2] = dest_in[2]; };

      void update (float time_step);

   private:

      MAP   *Map;
      float *cost;
      float *buffer;

      float position_x;
      float position_y;
      float position_z;

      int   dest[3];

      float max_speed;
      float speed;
      float direction;

      bool update_path;
      int  next_cell[3];

};

#endif
