#include "map.h"
#include "society.h"
#include "pathfinding.h"
#include "timer.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))

Society::Society (void)
{
   size[0] = 60;
   size[1] = 60;
   size[2] = 40;

   Map = new MAP (size);

   float dest[3];

   int map_layer = 20;

   int num_units = 100;

   current_unit_job_index = 0;
   current_job_index      = 0;

   const float *map = Map->access_ground ();

   int unit_count = 0;
   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            if (map[ind] >= 0.0f && ind_z == map_layer && unit_count < num_units) {
               unit_count++;
               dest[0] = (float)ind_x     + 0.5f;
               dest[1] = (float)ind_y     + 0.5f;
               dest[2] = (float)map_layer + 0.5f;

               units.push_back (new Unit (dest[0], dest[1], dest[2], Map));
            }
         }
      }
   }

   ibuffer = new   int[size[0] * size[1] * size[2]];
   fbuffer = new float[2 * size[0] * size[1] * size[2]];
}

Society::~Society (void)
{
   delete   Map;
   delete[] ibuffer;
   delete[] fbuffer;
}

// Destination is set by pooling out the available cells at and around the selected
// destination to provide a unique cell destination for each unit
void Society::set_destination (int destination[3])
{
   const float *map = Map->access_map ();

   int size[3];

   size[0] = Map->map_dim (0);
   size[1] = Map->map_dim (1);
   size[2] = Map->map_dim (2);

   int *cost_indices = ibuffer;

   int num_units = units.size();

   float *cost         = fbuffer;
   float *local_buffer = fbuffer + size[0] * size[1] * size[2];

   // Create the selected cost indices equal to the number of units
   cost_function2 (
         map,
         cost,
         cost_indices,
         size,
         destination,
         num_units,
         local_buffer);

   int *unit_dest_x = (int*)local_buffer;
   int *unit_dest_y = unit_dest_x + num_units;
   int *unit_dest_z = unit_dest_y + num_units;

   // Retrieve all unit destinations into an array for later use
   int ind = 0;
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      int unit_dest[3];

      unit->get_destination (unit_dest);

      unit_dest_x[ind] = unit_dest[0];
      unit_dest_y[ind] = unit_dest[1];
      unit_dest_z[ind] = unit_dest[2];

      ind++;
   }

   // Set unit destinations
   ind = 0;
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      if (!unit->is_selected()) continue;

      int dest_ind = cost_indices[ind++];

      int dest[3];

      dest[0] = ind_to_i (dest_ind, size[0], size[1], size[2]);
      dest[1] = ind_to_j (dest_ind, size[0], size[1], size[2]);
      dest[2] = ind_to_k (dest_ind, size[0], size[1], size[2]);

      // Determine if the select destination is already claimed by another unit
      bool found = true;
      for (int unit_ind = 0; unit_ind < num_units; unit_ind++)
      {
         if (
               dest[0] == unit_dest_x[unit_ind] &&
               dest[1] == unit_dest_y[unit_ind] &&
               dest[2] == unit_dest_z[unit_ind]
            )
         {
            found = false;
            break;
         }
      }

      if (found == false) continue;

      int location_value = Map->get_ground_cell (dest);

      if (location_value >= 0 && unit->is_selected ())
      {
         if (unit->num_jobs () > 0)
            queued_jobs.push_front (unit->pop_job ());

         unit->set_destination (dest);
      }
   }
}

void Society::update (float time_step)
{
   if (units.size () <= 0) return;

   const float *map = Map->access_ground ();

   float *cost         = fbuffer;
   float *local_buffer = fbuffer + size[0] * size[1] * size[2];

   if (current_unit_job_index >= units.size ()) current_unit_job_index = 0;

   // Assign ready jobs for available units
   Unit *unit = units.access (current_unit_job_index++);

   int unit_location[3] =
   { (int)unit->get_position (0),
      (int)unit->get_position (1),
      (int)unit->get_position (2) };

   // Assign a job for this unit if it has an available slot
   if (unit->available_job_slots ())
   {
      if (queued_jobs.size() > 0)
      {
         if (current_job_index >= queued_jobs.size ()) current_job_index = 0;
         Job *job = queued_jobs.access (current_job_index);

         // Test if this job has a ground-accessible cell near it by
         // first getting the job's cell location
         int job_location_cell[3] =
         { job->get_position (0),
            job->get_position (1),
            job->get_position (2) };

         // test if this cell is accessible to the unit
         bool accessible =
            cost_function (
                  map,
                  cost,
                  size,
                  job_location_cell,
                  unit_location,
                  local_buffer);

         if (accessible)
         {
            // the job is accessible to the unit, assign it to the unit
            unit->assign_job (job);

            // remove this job from the list since it now belongs to the unit
            queued_jobs.pop (current_job_index);
         }

         current_job_index++;
      }
   }

   // Update units
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      // Dismiss completed jobs by this unit
      if (unit->num_jobs() > 0)
      {
         Job *job = unit->access_job ();
         if (job->is_complete ()) {
            unit->pop_job ();
            delete job;
         }
      }

      // Update the unit's position and path planning
      unit->update (time_step);
   }

   Map->update ();
}

const float *Society::access_map ()
{
   return static_cast<const float*>(Map->access_map ());
}

void Society::select_units (int cell_selections[2][3], int map_layer, bool control_down)
{
   if (control_down == false)
   {
      for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
      {
         Unit *unit = units.access (unit_ind);

         unit->unselect();
      }
   }

   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      float x = unit->get_position (0);
      float y = unit->get_position (1);
      float z = unit->get_position (2);

      float min_x = cell_selections[0][0];
      float max_x = cell_selections[1][0];

      if (max_x < min_x) {
         float temp = min_x;
         min_x = max_x;
         max_x = temp;
      }

      float min_y = cell_selections[0][1];
      float max_y = cell_selections[1][1];

      if (max_y < min_y) {
         float temp = min_y;
         min_y = max_y;
         max_y = temp;
      }

      float min_z = cell_selections[0][2];
      float max_z = cell_selections[1][2];

      if (max_z < min_z) {
         float temp = min_z;
         min_z = max_z;
         max_z = temp;
      }

      max_z += 1.0f;

      if (x >= min_x && x <= max_x &&
          y >= min_y && y <= max_y &&
          z >= min_z && z < max_z)
      {
         unit->select();
      }
   }
}

void Society::select_all (void)
{
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);
      unit->select();
   }
}

void Society::unselect_all (void)
{
   // Unselect all units
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);
      unit->unselect ();
   }

   // Clear out the uncommitted jobs
   Map->unselect_uncommitted_jobs ();
}

void Society::set_group (int group_number)
{
   // Clear contents
   group[group_number].reset ();

   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      if (unit->is_selected ())
         group[group_number].push_front (unit);
   }
}

void Society::select_group (int group_number)
{
   // Unselect all units
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);
      unit->unselect();
   }

   for (int unit_ind = 0; unit_ind < group[group_number].size (); unit_ind++)
   {
      Unit *unit = group[group_number].access (unit_ind);
      unit->select ();
   }
}

void Society::select_cells (int cell_selections[2][3], bool control_down)
{
   Map->ready_jobs (cell_selections, control_down);
}

void Society::set_jobs (int job_type)
{

   int num_uncommitted_jobs;

   const int *new_job_location_ind =
      Map->access_uncommitted_jobs (&num_uncommitted_jobs);

   for (int ind = 0; ind < num_uncommitted_jobs; ind++)
   {
      bool duplicate = false;

      // Test if this is already in the committed-jobs list
      for (int job_ind = 0; job_ind < queued_jobs.size (); job_ind++) {
         Job *job = queued_jobs.access (job_ind);
         int location_ind = job->get_flattened_index ();
         if (new_job_location_ind[ind] == location_ind) duplicate = true;
      }

      if (duplicate) continue;

      // Test if this is already assigned to one of the units
      for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
      {
         Unit *unit = units.access (unit_ind);
         for (int job_ind = 0; job_ind < unit->num_jobs(); job_ind++)
         {
            Job *job = unit->access_job (job_ind);
            int location_ind = job->get_flattened_index ();
            if (new_job_location_ind[ind] == location_ind) duplicate = true;
         }
      }

      if (duplicate) continue;

      int location[3];
      location[2] = new_job_location_ind[ind] / (size[0] * size[1]);
      location[1] = new_job_location_ind[ind] / size[0] % size[1];
      location[0] = new_job_location_ind[ind] % size[0];

      queued_jobs.push_front (new Job (new_job_location_ind[ind], location, job_type));
   }

   Map->unselect_uncommitted_jobs ();
}
