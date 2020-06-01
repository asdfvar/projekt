#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <thread>
#include "map.h"
#include "jobs.h"
#include "item.h"
#include "job_manager.h"

class Unit
{
   public:

      Unit (float position[3], MAP*);

     ~Unit (void);

      float get_position (int ind) { return position[ind]; };
      float get_speed (void) { return speed; };
      void get_destination (int *dest_out);
      void set_destination (int dest_in[3]);
      void unselect (void) { selected = false; };
      void select (void)   { selected = true;  };
      void select (int bounding_box[2][3]);
      bool is_selected (void) { return selected; };
      void update (float time_step);
      void add_item (int type);
      void assign_job (Job *job);
      Job *access_job (int ind) { return jm.access_job (ind); };
      Job *access_active_job (void) { return jm.access_active_job (); };
      bool available_job_slots (void);
      int num_jobs (void) { return jm.num_jobs (); };
      int num_return_jobs (void) { return jm.num_return_jobs (); };
      int num_total_jobs (void) { return jm.num_total_jobs (); };
      Job *pop_return_job (void);
      Job *pop_active_job (void) { return jm.pop_active_job (); };
      Job *return_job (void)     { return jm.pop_return_job (); };
      virtual void draw (float *transform, float *translation) {};

   protected:

      unsigned int tic;

      enum State_type { STANDBY, MOVING, FALLING };
      State_type state;
      MAP   *Map;
      float *cost;
      float *buffer;
      Job_manager jm;
      Container<Item> items;
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
      bool solution_found;
};

#endif
