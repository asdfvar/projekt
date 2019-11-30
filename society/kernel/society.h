#ifndef SOCIETY_H
#define SOCIETY_H

#include "map.h"
#include <queue>
#include <vector>
#include "unit.h"

class Society
{
   public:

      Society (void);
      ~Society (void);

      void update (float time_step);

      void set_destination (int dest[3]);

      const float *access_map (int *dim_x, int *dim_y, int *dim_z);

      int get_unit_positions (float *x, float *y, float *z);

   private:

      MAP *Map;
      int  dim_x;
      int  dim_y;
      int  dim_z;

      float *cost;
      float *buffer;

      std::vector<Unit*> units;
};

#endif
