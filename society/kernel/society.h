#ifndef SOCIETY_H
#define SOCIETY_H

#include <queue>
#include "control.h"

class Society
{
   public:
      Society (void);
      ~Society (void);

      void input (Control *control);
      void update (float time_step);

      const float *access_map (int *dim_x, int *dim_y, int *dim_z);

   private:
      float *map;
      int dim_x;
      int dim_y;
      int dim_z;

      std::queue<Control*> control_queue;
};

#endif
