#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <thread>
#include "map.h"

typedef struct PATH_FUNC_ARGS
{
   const float       *map;
   int                dim[3];
   int                start[3];
   int                dest_in[3];
   int               *dest;
   int               *residency;
   int               *path;
   int               *path_size;
   bool               done;
   pthread_barrier_t *barrier;
} PATH_FUNC_ARGS;

void *path_finding_func (void *path_func_args_in);

class Unit {

   public:

      Unit (float position_x,
            float position_y,
            float position_z,
            MAP*);

     ~Unit (void);

      float get_position (int ind) { return position[ind]; };

      float get_speed (void) { return speed; };

      void get_destination (int *dest_out);

      void set_destination (int dest_in[3]);

      void unselect (void) { selected = false; };
      void select (void)   { selected = true;  };

      bool is_selected (void) { return selected; };

      void update (float time_step);

   private:

      MAP   *Map;

      int   *path;

      float *cost;
      float *buffer;
      int *scratch;

      float position[3];

      int   dest[3];
      int   residency[3];

      float max_speed;
      float speed;
      float direction;

      bool selected;

      int path_size;

      PATH_FUNC_ARGS    path_func_args;
      pthread_t         path_planner_thread;
      pthread_barrier_t barrier;
};

#endif
