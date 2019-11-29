#ifndef SOCIETY_H
#define SOCIETY_H

#include "map.h"
#include <queue>
#include <vector>
#include "control.h"
#include "unit.h"

class Society
{
   public:

      Society (void);
      ~Society (void);

      void update (float time_step);

      void set_destination (int dest[3])
      {
         destination[0] = dest[0];
         destination[1] = dest[1];
         destination[2] = dest[2];

         units[0]->set_destination (destination);
      }

      const float *access_map (int *dim_x, int *dim_y, int *dim_z);
      int get_unit_positions (float *x, float *y, float *z);

   private:

      MAP *Map;
      int  dim_x;
      int  dim_y;
      int  dim_z;
      int  destination[3];

      float *cost;
      float *buffer;

      std::vector<Unit*> units;
};

#endif
