#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include "map.h"
#include "unit.h"
#include "utils.h"
#include "timer.h"
#include "item.h"
#include "pathfinding.h"

Unit::Unit (
      float position_in[3],
      MAP *Map_in)
{
   tic   = rand ();

   jm.initialize ();

   state = STANDBY;

   direction = 0.0f;

   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];

   Map = Map_in;

   int size[3] = { Map->shape (0), Map->shape (1), Map->shape (2) };

   path   = new   int[size[0] * size[1] * size[2]];
   cost   = new float[size[0] * size[1] * size[2]];
   buffer = new float[size[0] * size[1] * size[2]];

   trim_path_end = false;

   dest[0] = position[0];
   dest[1] = position[1];
   dest[2] = position[2];

   selected = false;

   path_size = 0;

   speed = 10.0f;

   min_job_dist2 = 3.01f; // greater than sqrt (3)

   power = 0.2f;

   residency[0] = 0;
   residency[1] = 0;
   residency[2] = 0;

   // Path to job location
   solution_found = true;
}

Unit::~Unit (void)
{
   delete[] path;
   delete[] cost;
   delete[] buffer;
}

void Unit::set_destination (int dest_in[3])
{
   const bool  *ground_map = Map->access_ground ();
   const float *weight_map = Map->access_weight ();

   int dim[3];

   dim[0] = Map->shape (0);
   dim[1] = Map->shape (1);
   dim[2] = Map->shape (2);

   int start[3];

   start[0] = (int)position[0];
   start[1] = (int)position[1];
   start[2] = (int)position[2];

   // start determining a new path to the destination

   // Find the path to the destination.
   // The cost function is produced with the destination
   // as the start index for the purpose of descending to the destination
   solution_found = cost_function (
         ground_map,
         weight_map,
         cost,
         dim,
         dest_in,
         start,
         buffer);

   // Return and don't do anything if a solution is not found
   if (!solution_found) return;

   // Compute the path based on the cost function
   path_size = pathfinding (
         ground_map,
         cost,
         dim,
         start,
         dest_in,
         path);

   if (trim_path_end)
   {
      bool done = false;
      for (path_size = 0; !done; path_size += 1)
      {
         int path_pos[3] = {
            flat_ind_to_dim (0, path[path_size], dim),
            flat_ind_to_dim (1, path[path_size], dim),
            flat_ind_to_dim (2, path[path_size], dim) };

         float dist2 =
            (float)((dest_in[0] - path_pos[0]) *
            (dest_in[0] - path_pos[0]) +
            (dest_in[1] - path_pos[1]) *
            (dest_in[1] - path_pos[1]) +
            (dest_in[2] - path_pos[2]) *
            (dest_in[2] - path_pos[2]));

         if (dist2 <= min_job_dist2) done = true;
      }

      path_size -= 1; // Undo the loop incrementer

      dest[0] = flat_ind_to_dim (0, path[path_size], dim);
      dest[1] = flat_ind_to_dim (1, path[path_size], dim);
      dest[2] = flat_ind_to_dim (2, path[path_size], dim);

      trim_path_end = false;
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

   state = MOVING;
};

void Unit::select (int bounding_box[2][3])
{
   if (
         position[0] >= bounding_box[0][0] && position[0] <= bounding_box[1][0] &&
         position[1] >= bounding_box[0][1] && position[1] <= bounding_box[1][1] &&
         Map->test_los (position))
   {
      selected = true;
   }
}

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
   if (state == STANDBY)
   {
      // Check if this unit is going to fall
      int position_cell[3] = {
         (int)position[0],
         (int)position[1],
         (int)position[2] };

      bool ground_cell_value = Map->get_ground_cell (position_cell);
      bool air_cell_value    = Map->get_air_cell    (position_cell);

      if (ground_cell_value == false && air_cell_value == true)
      {
         // Set the state to "falling"
         state = FALLING;
         return;
      }

      // Check if this unit is in an invalid block
      if (ground_cell_value == false || air_cell_value == false)
      {
         // Relinquish this unit from all its jobs
         jm.set_return_all_jobs ();

std::cout << __FILE__ << __LINE__ << ":unit " << this << " is in an invalid block" << std::endl;
#if 0
         // Push the unit upwards until it's no longer inside a block
         for (ground_cell_value = Map->get_ground_cell (position_cell);
               ground_cell_value == false; position_cell[2] += 1)
         {
            position[2] += 1.0f;
         }
#else
         int size[3] = {
            Map->shape (0),
            Map->shape (1),
            Map->shape (2) };

         const bool *ground_cells = Map->access_ground ();
         int flat_ind = search (ground_cells, size, position_cell);
         position[0] = static_cast<float> (flat_ind_to_dim (0, flat_ind, size));
         position[1] = static_cast<float> (flat_ind_to_dim (1, flat_ind, size));
         position[2] = static_cast<float> (flat_ind_to_dim (2, flat_ind, size));
#endif
std::cout << __FILE__ << __LINE__ << ":resolved unit " << this << " invalid block issue" << std::endl;
      }

      // Check for internal queued jobs
      if (jm.num_jobs () > 0)
      {
         int job_location[3];

         // Return any jobs that don't have a path defined to their destination
         if (!solution_found)
            jm.set_return_last_job ();

         job_location[0] = jm.get_active_job_position (0);
         job_location[1] = jm.get_active_job_position (1);
         job_location[2] = jm.get_active_job_position (2);

         // if location is within tolerance of the job location, work on it
         float dist2 =
            (floorf (position[0]) - (float)job_location[0]) * (floorf (position[0]) - (float)job_location[0]) +
            (floorf (position[1]) - (float)job_location[1]) * (floorf (position[1]) - (float)job_location[1]) +
            (floorf (position[2]) - (float)job_location[2]) * (floorf (position[2]) - (float)job_location[2]);

         if (dist2 <= min_job_dist2)
            jm.active_job_action (power * time_step);
         else
         {
            // Set the final destination to the cell that is within reach by this unit to the job location
            trim_path_end = true;

            // set destination (note that this runs in a separate thread)
            set_destination (job_location);
         }
      }
   }

   // Update the position
   else if (state == MOVING)
   {
      // Update position info

      int dim[3];

      dim[0] = Map->shape (0);
      dim[1] = Map->shape (1);
      dim[2] = Map->shape (2);

      float local_dest[3];

      // If the unit is in the local destination cell,
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

         next_cell[0] = flat_ind_to_dim (0, path[0], dim);
         next_cell[1] = flat_ind_to_dim (1, path[0], dim);
         next_cell[2] = flat_ind_to_dim (2, path[0], dim);

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
            state = STANDBY;
            return;
         }

         // Adjust time step to the remaining time
         time_step -= sqrtf (dist2) / speed;

         int next_cell[3];

         // Update the local destination to the next available cell
         next_cell[0] = flat_ind_to_dim (0, path[0], dim);
         next_cell[1] = flat_ind_to_dim (1, path[0], dim);
         next_cell[2] = flat_ind_to_dim (2, path[0], dim);

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
   else if (state == FALLING)
   {
      // Fall if there isn't ground space to support the unit
      int unit_position[3] = { (int)position[0], (int)position[1], (int)position[2] };
      if (Map->get_ground_cell (unit_position) == false && Map->get_air_cell (unit_position) == true)
      {
         position[2] -= 1.0f;
      }
      else
      {
         state = STANDBY;
      }
   }

   tic++;
}

void Unit::add_item (int type)
{
   items.push_front (new Item (type));
}

void Unit::assign_job (Job *job)
{
   jm.assign_job (job);
}

bool Unit::available_job_slots (void)
{
   return jm.available_job_slots ();
}

Job *Unit::pop_return_job (void)
{
   state      = STANDBY;

   jm.set_return_all_jobs ();

   return jm.pop_return_job ();
}
