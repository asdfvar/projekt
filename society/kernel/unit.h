#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <thread>
#include "map.h"
#include "jobs.h"
#include "container.h"

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
   bool              *trim_path_end;
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

      void assign_job (Job *job);

      Job *access_job (int ind) { return jobs.access (ind); };

      Job *access_active_job (void) { return jobs.back (); };

      bool available_job_slots (void);

      int num_jobs (void) { return jobs.size (); };

      Job *pop_job (void) { return jobs.pop_back (); };

      bool is_forfeiting_jobs (void) { return relinquish_jobs; };
      void ready (void) { relinquish_jobs = false; };

   private:

      MAP   *Map;

      Job   *active_job;

      int   *path;
      bool   trim_path_end;

      float position[3];

      int   dest[3];
      int   residency[3];

      float speed;

      float direction;

      float power;

      bool selected;

      int path_size;

      int jobs_limit;
      Container<Job> jobs;

      bool relinquish_jobs;

      PATH_FUNC_ARGS    path_func_args;
      pthread_t         path_planner_thread;
      pthread_barrier_t barrier;
};

#endif
