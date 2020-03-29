#ifndef X01_H
#define X01_H

#include "unit.h"

class X01 : public Unit
{
   public:
      X01 (
            float position_x,
            float position_y,
            float position_z,
            MAP* Map);

      void draw (float *transform, float *translation) override;
};

class X02 : public Unit
{
   public:
      X02 (
            float position_x,
            float position_y,
            float position_z,
            MAP* Map);

      void draw (float *transform, float *translation) override;
};

#endif
