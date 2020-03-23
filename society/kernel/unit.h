#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <thread>
#include "map.h"
#include "jobs.h"
#include "container.h"

class Unit
{
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

      Job *pop_active_job (void);

      int return_jobs_size (void) { return return_jobs.size (); };
      Job *return_job (void) { return return_jobs.pop_back (); };

      virtual void draw (float *transform, float *translation) {};

   protected:

      unsigned int tic;

      enum State_type { STANDBY, MOVING, FALLING };
      State_type state;

      MAP   *Map;
      float *cost;
      float *buffer;

      Job   *active_job;

      int   *path;
      bool   trim_path_end;

      float position[3];

      int   dest[3];
      int   residency[3];

      float speed;

      float direction;

      float min_job_dist2;

      float power;

      bool selected;

      int path_size;

      int jobs_limit;
      Container<Job> jobs;
      Container<Job> return_jobs;

      bool solution_found;
};

#endif
