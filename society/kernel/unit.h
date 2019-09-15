#ifndef UNIT_H
#define UNIT_H

class Unit {

   public:
      Unit (float location_x, float location_y, float location_z);

      float get_location_x (void) { return location_x; };
      float get_location_y (void) { return location_y; };
      float get_location_z (void) { return location_z; };

   private:
      float location_x;
      float location_y;
      float location_z;

};

#endif
