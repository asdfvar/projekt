#ifndef RAY_TRACE_H
#define RAY_TRACE_H

#include "ray.h"

class RayTrace {

   public:

      RayTrace (
             float start_position[3],
             float start_look_direction[3],
             float window_width,
             float window_height,
             float window_distance,
             float dx,
             float dy);

      ~RayTrace (void);

      void insert_sphere (
             float position[3],
             float radius);

      void execute (unsigned int num_threads);

   private:

      Ray *grid;

};

#endif
