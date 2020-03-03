#include <iostream>
#include <iomanip>
#include "map.h"
#include "unit.h"
#include "timer.h"
#include <cmath>
#include <cstdlib>
#include "pathfinding.h"

Unit::Unit (
      float position_x_in,
      float position_y_in,
      float position_z_in,
      MAP *Map_in)
{
   state = 0;

   direction = 0.0f;

   position[0] = position_x_in;
   position[1] = position_y_in;
   position[2] = position_z_in;

   jobs_limit = 1;
   active_job = 0;

   Map = Map_in;

   int map_dims[3] = { Map->map_dim (0), Map->map_dim (1), Map->map_dim (2) };

   path = new int[map_dims[0] * map_dims[1] * map_dims[2]];
   trim_path_end = false;

   dest[0] = position[0];
   dest[1] = position[1];
   dest[2] = position[2];

   selected = false;

   path_size = 0;

   speed = 10.0f;

   power = 0.2f;

   residency[0] = 0;
   residency[1] = 0;
   residency[2] = 0;

   // Path to job location
   solution_found = true;

   // Create the barrier for the path-finding
   pthread_barrier_init (&barrier, NULL, 2);

   path_func_args.map            =  Map->access_ground ();
   path_func_args.dim[0]         =  map_dims[0];
   path_func_args.dim[1]         =  map_dims[1];
   path_func_args.dim[2]         =  map_dims[2];
   path_func_args.dest           =  dest;
   path_func_args.residency      =  residency;
   path_func_args.path_size      = &path_size;
   path_func_args.path           =  path;
   path_func_args.trim_path_end  = &trim_path_end;
   path_func_args.barrier        = &barrier;
   path_func_args.solution_found = &solution_found;
   path_func_args.done           =  false;

   pthread_create (
         &path_planner_thread,
         NULL,
         path_finding_func,
         (void*)&path_func_args);
}

Unit::~Unit (void)
{
   path_func_args.done = true;

   // join the path-planning thread
   pthread_join (path_planner_thread, NULL);

   pthread_barrier_destroy (&barrier);

   delete[] path;
}

void Unit::set_destination (int dest_in[3])
{
   const float *map = Map->access_ground ();

   int dim[3];

   dim[0] = Map->map_dim (0);
   dim[1] = Map->map_dim (1);
   dim[2] = Map->map_dim (2);

   int start[3];

   start[0] = (int)position[0];
   start[1] = (int)position[1];
   start[2] = (int)position[2];

   path_func_args.start[0]   = start[0];
   path_func_args.start[1]   = start[1];
   path_func_args.start[2]   = start[2];

   path_func_args.dest_in[0] = dest_in[0];
   path_func_args.dest_in[1] = dest_in[1];
   path_func_args.dest_in[2] = dest_in[2];

   // Signal to the path-finding to start determining a new path
   // to the destination
   pthread_barrier_wait (&barrier);
   pthread_barrier_wait (&barrier); // TODO: remove this

   state = 1;
};

void Unit::get_destination (int *dest_out)
{
   dest_out[0] = dest[0];
   dest_out[1] = dest[1];
   dest_out[2] = dest[2];
}

// Update unit position and path planning
void Unit::update (float time_step)
{
   // Static state
   if (state == 0)
   {
      // Check if this unit is going to fall
      int position_cell[3] = {
         (int)position[0],
         (int)position[1],
         (int)position[2] };

      float ground_cell_value = Map->get_ground_cell (position_cell);

      if (ground_cell_value < 0.0f)
      {
         // Set the state to "falling"
         state = 2;
         return;
      }

      // Check for internal queued jobs
      if (jobs.size () > 0)
      {
         int job_location[3];

         const float min_job_dist = 4.1f;

         if (jobs.size () > 0 && active_job != jobs.back ())
         {
            active_job = jobs.back ();

            job_location[0] = active_job->get_position (0);
            job_location[1] = active_job->get_position (1);
            job_location[2] = active_job->get_position (2);

         }

         // Return any jobs that don't have a path defined to their destination
         if (!solution_found)
            return_jobs.push_front (jobs.pop_back ());

         job_location[0] = active_job->get_position (0);
         job_location[1] = active_job->get_position (1);
         job_location[2] = active_job->get_position (2);

         // if location is within tolerance of the job location, work on it
         float dist2 =
            (floorf (position[0]) - (float)job_location[0]) * (floorf (position[0]) - (float)job_location[0]) +
            (floorf (position[1]) - (float)job_location[1]) * (floorf (position[1]) - (float)job_location[1]) +
            (floorf (position[2]) - (float)job_location[2]) * (floorf (position[2]) - (float)job_location[2]);

         if (dist2 <= min_job_dist)
            active_job->act (power * time_step);
         else
         {
            // TODO: find a different way of handling this (something different than "trimming the end of the path")
            // Set the final destination to one cell adjacent to the job location
            trim_path_end = true;

            // set destination (note that this runs in a separate thread)
            set_destination (job_location);
         }
      }
   }

   // Update the position
   else if (state == 1)
   {
      // Update position info

      int dim[3];

      dim[0] = Map->map_dim (0);
      dim[1] = Map->map_dim (1);
      dim[2] = Map->map_dim (2);

      float local_dest[3];

      // Condition check if the unit is in the local destination cell,
      // set the local destination to the center of the cell
      if (path_size <= 0)
      {
         local_dest[0] = (float)dest[0] + 0.5f;
         local_dest[1] = (float)dest[1] + 0.5f;
         local_dest[2] = (float)dest[2] + 0.5f;
      }
      else
      {
         int next_cell[3];

         next_cell[0] = (path[0] % dim[0]);
         next_cell[1] = (path[0] % (dim[0] * dim[1])) / dim[0];
         next_cell[2] = path[0] / (dim[0] * dim[1]);

         local_dest[0] = (float)next_cell[0] + 0.5f;
         local_dest[1] = (float)next_cell[1] + 0.5f;
         local_dest[2] = (float)next_cell[2] + 0.5f;
      }

      float dist2 =
         (position[0] - local_dest[0]) * (position[0] - local_dest[0]) +
         (position[1] - local_dest[1]) * (position[1] - local_dest[1]) +
         (position[2] - local_dest[2]) * (position[2] - local_dest[2]);

      // set the position to the local destination if the projection would otherwise
      // surpass it
      float comp_dist = speed * time_step;
      while (dist2 <= comp_dist * comp_dist)
      {
         position[0] = local_dest[0];
         position[1] = local_dest[1];
         position[2] = local_dest[2];

         // Remove the traversed path element from the list.
         // Return when the destination has been reached (which happens
         // when the path size is zero)
         for (int ind = 0; ind < path_size; ind++) path[ind] = path[ind+1];

         if (path_size > 0) path_size--;
         if (path_size <= 0)
         {
            state = 0;
            return;
         }

         // Adjust time step to the remaining time
         time_step -= sqrtf (dist2) / speed;

         int next_cell[3];

         // Update the local destination to the next available cell
         next_cell[0] = (path[0] % dim[0]);
         next_cell[1] = (path[0] % (dim[0] * dim[1])) / dim[0];
         next_cell[2] = path[0] / (dim[0] * dim[1]);

         local_dest[0] = (float)next_cell[0] + 0.5f;
         local_dest[1] = (float)next_cell[1] + 0.5f;
         local_dest[2] = (float)next_cell[2] + 0.5f;

         dist2 =
            (position[0] - local_dest[0]) * (position[0] - local_dest[0]) +
            (position[1] - local_dest[1]) * (position[1] - local_dest[1]) +
            (position[2] - local_dest[2]) * (position[2] - local_dest[2]);

         comp_dist = speed * time_step;
      }

      float step[3];

      float diff_pos[3];

      // Update the position

      diff_pos[0] = local_dest[0] - position[0];
      diff_pos[1] = local_dest[1] - position[1];
      diff_pos[2] = local_dest[2] - position[2];

      float theta = atan2f (diff_pos[1], diff_pos[0]);

      position[0] += speed * cosf (theta) * time_step;
      position[1] += speed * sinf (theta) * time_step;

      theta = atan2f (diff_pos[2], sqrtf (diff_pos[0] * diff_pos[0] + diff_pos[1] * diff_pos[1]));

      position[2] += speed * sinf (theta) * time_step;
   }

   // Fall
   else if (state == 2)
   {
      // Fall if there isn't ground space to support the unit
      int unit_position[3] = { (int)position[0], (int)position[1], (int)position[2] };
      if (Map->get_ground_cell (unit_position) < 0.0f && Map->get_air_cell (unit_position) >= 0.0f)
      {
         position[2] -= 1.0f;
      }
      else
      {
         state = 0;
      }

   }
}

void Unit::assign_job (Job *job)
{
   if (!available_job_slots ())
   {
      std::cout
         << "Not enough available job slots"
         << " available for unit "
         << this << std::endl;

      return;
   }

   jobs.push_front (job);
}

bool Unit::available_job_slots (void)
{
   if (jobs.size () >= jobs_limit)
      return false;

   return true;
}

void *path_finding_func (void *path_func_args_in)
{
   PATH_FUNC_ARGS *path_func_args = (PATH_FUNC_ARGS*)path_func_args_in;

   const float *map           =  path_func_args->map;
   int   *dim                 =  path_func_args->dim;
   int   *start               =  path_func_args->start;
   int   *dest_in             =  path_func_args->dest_in;
   int   *dest                =  path_func_args->dest;
   int   *residency           =  path_func_args->residency;
   int   *path                =  path_func_args->path;
   int   *path_size           =  path_func_args->path_size;
   bool  *trim_path_end       =  path_func_args->trim_path_end;
   bool  *done                = &path_func_args->done;
   bool  *solution_found      =  path_func_args->solution_found;
   pthread_barrier_t *barrier =  path_func_args->barrier;

   float *cost   = new float[dim[0] * dim[1] * dim[2]];
   float *buffer = new float[dim[0] * dim[1] * dim[2]];

   while (!*done)
   {
      // Signal from the main thread to start determining a new path
      pthread_barrier_wait (barrier);

      // Test if done
      if (*done) continue;

      // Find the path to the destination.
      // The cost function is produced with the destination
      // as the start index for the purpose of descending to the destination
      *solution_found = cost_function (
            map,
            cost,
            dim,
            dest_in,
            start,
            buffer);

      // Don't do anything if a solution is not found
      if (!*solution_found) continue;

      // Compute the path based on the cost function
      *path_size = pathfinding (
            cost,
            dim,
            start,
            dest_in,
            path);

      if (*trim_path_end)
      {
         *path_size -= 1;
         dest[0] = path[*path_size] % dim[0];
         dest[1] = path[*path_size] % (dim[0] * dim[1]) / dim[0];
         dest[2] = path[*path_size] / (dim[0] * dim[1]);

         *trim_path_end = false;
      }
      else
      {
         dest[0] = dest_in[0];
         dest[1] = dest_in[1];
         dest[2] = dest_in[2];
      }

      residency[0] = dest_in[0];
      residency[1] = dest_in[1];
      residency[2] = dest_in[2];

      pthread_barrier_wait (barrier); // TODO: remove this
   }
}
