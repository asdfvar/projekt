#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "container.h"
#include "jobs.h"

class Job_manager
{
   public:

      Job_manager (void)
      {
         jobs_limit = 1;
         active_job = 0;
      };

      void assign_job (Job *job)
      {
         if (jobs_list.size () >= jobs_limit)
         {
            std::cout
               << "Not enough available job slots"
               << std::endl;
            return;
         }

         jobs_list.push_front (job);
      };

      bool available_job_slots (void)
      {
         if (jobs_list.size () >= jobs_limit)
            return false;

         return true;
      };

      int num_jobs (void)
      {
         return jobs_list.size ();
      };

      int num_return_jobs (void)
      {
         return return_jobs.size ();
      };

      int num_total_jobs (void)
      {
         return jobs_list.size () + return_jobs.size ();
      };

      Job *pop_return_job (void)
      {
         if (return_jobs.size () > 0)
            return return_jobs.back ();

         else return nullptr;
      };

      Job *pop_active_job (void)
      {
         if (jobs_list.size () > 0) return jobs_list.pop_back ();
         else return nullptr;
      };

      void set_return_last_job (void)
      {
         if (jobs_list.size () > 0) return_jobs.push_front (jobs_list.pop_back ());
      };

      void set_return_all_jobs (void)
      {
         while (jobs_list.size () > 0) return_jobs.push_front (jobs_list.pop_back ());
      };

      int get_active_job_position (int ind)
      {
         if (jobs_list.size () > 0) return jobs_list.back ()->get_position (ind);
         else return -1;
      };

      void active_job_action (float energy)
      {
         if (jobs_list.size () > 0) jobs_list.back ()->act (energy);
      };

      Job *access_job (int ind)
      {
         return jobs_list.access (ind);
      };

      Job *access_active_job (void) { return jobs_list.back (); };

private:

      Job *active_job;

      int jobs_limit;
      Container<Job> jobs_list;
      Container<Job> return_jobs;
};


#endif
