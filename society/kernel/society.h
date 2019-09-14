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

   private:
      float *map;
      std::queue<Control*> control_queue;
};

#endif
