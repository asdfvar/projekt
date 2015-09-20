#ifndef RAY_TRACE_H
#define RAY_TRACE_H

#include <list>
#include "ray.h"
#include "assembly.h"
#include "light_source.h"

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

      void insert_light (
             float position[3],
             float intensity[3]);

      void insert_sphere (
             float position[3],
             float radius);

      void run ( unsigned int num_threads );

      void execute (unsigned int thread_id,
                    unsigned int num_threads);

      int get_nxy ( void );

      void print_grid ( void );

      void write_grid ( void );

      Ray      *grid;

   private:

      bool intersect_objects (
                  Ray    in_ray,
                  Ray   *new_ray,
                  float *distance);

      float                   position[3];
      Assembly                all_objects;
      std::list<Light_source> lights;
      int nx, ny;

};

#endif
