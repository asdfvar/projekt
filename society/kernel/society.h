#ifndef SOCIETY_H
#define SOCIETY_H

#include <queue>
#include <vector>
#include "control.h"
#include "unit.h"

class Society
{
   public:
      Society (void);
      ~Society (void);

      void input (Control *control);
      void update (float time_step);

      const float *access_map (int *dim_x, int *dim_y, int *dim_z);
      int get_unit_positions (float *x, float *y, float *z);

   private:
      float *map;
      int dim_x;
      int dim_y;
      int dim_z;
      int destination[3];

      float *cost;
      float *buffer;

      std::queue<Control*> control_queue;
      std::vector<Unit> units;
};

#endif
