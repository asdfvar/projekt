#ifndef JOBS_H
#define JOBS_H

/* Jobs for units to take. These jobs are defined as
 ** static in nature (usually performed on objects that
 ** don't move in the environment)
 */
class Job
{
   public:

      Job (int flattened_index, int location_in[3], int type);

      bool is_complete (void) { return complete; };

      int get_position (int ind) { return location[ind]; };

      int get_flattened_index (void) { return flattened_index; };

   protected:

      int   type;            // type of job
      int   location[3];     // location the job takes place
      int   flattened_index; // index (flattened location)
      float duration;        // duration it takes to complete the job (seconds)
      float time_spent;      // total time spent working on the task
      bool  complete;        // flag to determine if the job is complete
};

#endif
