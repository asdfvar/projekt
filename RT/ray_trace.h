#ifndef RAY_TRACE_H
#define RAY_TRACE_H

#include "ray.h"

class RayTrace {

   public:

      Ray *grid;

      RayTrace (
             float start_position[3],
             float start_look_direction[3],
             float window_width,
             float window_height,
             float window_distance,
             float dx,
             float dy);

      ~RayTrace (void);

   private:

};

#endif
