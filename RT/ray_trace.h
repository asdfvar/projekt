#ifndef RAY_TRACE_H
#define RAY_TRACE_H

#include "ray.h"
#include "assembly.h"

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

      void run ( unsigned int num_threads );

      void execute (unsigned int num_threads);

   private:

      float     position[3];
      Ray      *grid;
      Assembly  all_objects;
      int nxy;

};

#endif
