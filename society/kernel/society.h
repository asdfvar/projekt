#ifndef SOCIETY_H
#define SOCIETY_H

#include "control.h"

class Society
{
   public:
      Society (void);
      ~Society (void);

      void controls (struct Keyboard keyboard, struct Mouse mouse);

   private:
      float *map;
};

#endif
