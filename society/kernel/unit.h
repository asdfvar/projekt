#ifndef UNIT_H
#define UNIT_H

class Unit {

   public:
      Unit (float position_x, float position_y, float position_z);

      float get_position_x (void) { return position_x; };
      float get_position_y (void) { return position_y; };
      float get_position_z (void) { return position_z; };

      void set_speed (float speed_in);

      void move (float time_step, float direction);

   private:
      float position_x;
      float position_y;
      float position_z;

      float speed;

};

#endif
