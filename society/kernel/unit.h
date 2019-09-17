#ifndef UNIT_H
#define UNIT_H

class Unit {

   public:
      Unit (float position_x, float position_y, float position_z);

      float get_position_x (void) { return position_x; };
      float get_position_y (void) { return position_y; };
      float get_position_z (void) { return position_z; };

      void move (float time_step);

   private:
      float position_x;
      float position_y;
      float position_z;

      float speed;
      float direction;

};

#endif
