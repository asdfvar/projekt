#ifndef JOBS_H
#define JOBS_H

/* Jobs for units to take. These jobs are defined as
 ** static in nature (usually performed on objects that
 ** don't move in the environment)
 */

namespace jid
{
   enum jid { REMOVE, BUILD };
}

class Job
{
   public:

      Job (int flattened_index, int location_in[3]);
      bool is_complete (void) { return complete; };
      int get_position (int ind) { return location[ind]; };
      int get_flattened_loc_index (void) { return flattened_loc_index; };
      int get_type (void) { return job_type; };
      unsigned int get_material (void) { return material; };
      void act (float power);

   protected:

      void init (int flattened_index, int location_in[3]);

      enum jid::jid    job_type;
      int              location[3];         // location the job takes place
      int              flattened_loc_index; // index (flattened location)
      float            work;                // Work towards completing the job (Joules)
      float            energy;              // Energy needed to complete the job (Joules)
      bool             complete;            // flag to determine if the job is complete
      unsigned int     material;
};

class JobRemove : public Job
{
   public:

      JobRemove (
            int          flattened_index,
            int          location_in[3],
            unsigned int material);
};

class JobBuild : public Job
{
   public:

      JobBuild (
            int          flattened_index,
            int          location_in[3],
            unsigned int material);
};

#endif
