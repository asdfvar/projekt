#include "map.h"
#include "society.h"
#include "pathfinding.h"
#include "timer.h"
#include "utils.h"
#include "derivative_units.h"
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

   current_job_unit_index = 0;
   current_job_index      = 0;

   const bool *ground_map = Map->access_ground ();

   ibuffer = new   int[    size[0] * size[1] * size[2]];
   fbuffer = new float[2 * size[0] * size[1] * size[2]];
}

Society::~Society (void)
{
   while (units.size () > 0)
      delete units.pop_back ();

   delete   Map;
   delete[] ibuffer;
   delete[] fbuffer;
}

void Society::set_view (float transform[4], float translation[2])
{
   // Random (x,y) location in the window [-1, 1]
   float rx = (float)(rand () % 1000) / 1000.0f;
   float ry = (float)(rand () % 1000) / 1000.0f;

   rx = 2.0f * rx - 1.0f;
   ry = 2.0f * ry - 1.0f;

   // Convert the random (x,y) location to the equivalent map location
   float x = window_to_map (rx, size[0]);
   float y = window_to_map (ry, size[1]);

   float initial_x = x;
   float initial_y = y;

   const bool  *ground_map = Map->access_ground ();

   const int num_units = 6;

   for (int unit_ind = 0; unit_ind < num_units; unit_ind++)
   {
      int cell_ind[3] = { (int)x, (int)y, size[2] - 1 };

      for (int layer = size[2] - 1; !Map->get_ground_cell (cell_ind) && layer >= 0; layer--)
         cell_ind[2] = layer;

      float location[3] = {
         static_cast<float> (cell_ind[0]),
         static_cast<float> (cell_ind[1]),
         static_cast<float> (cell_ind[2]) };

      if (unit_ind % 2 == 1)
         units.push_back (new X01 (location, Map));
      else
         units.push_back (new X02 (location, Map));

      // TODO: do this better and more robustly
      x += 1.0f;
   }

   int cell_ind[3] = { (int)initial_x, (int)initial_y, size[2] - 1 };

   for (int layer = size[2] - 1; !Map->get_ground_cell (cell_ind) && layer >= 0; layer--)
      cell_ind[2] = layer;

   set_destination (cell_ind, false);

   transform[0] = 4.0f;
   transform[3] = 4.0f;

   translation[0] = -rx;
   translation[1] = -ry;
}

// Destination is set by pooling out the available cells at and around the selected
// destination to provide a unique cell destination for each unit
void Society::set_destination (int destination[3], bool selected_units)
{
   const bool  *ground_map = Map->access_ground ();
   const float *weight_map = Map->access_weight ();

   int size[3];

   size[0] = Map->shape (0);
   size[1] = Map->shape (1);
   size[2] = Map->shape (2);

   int *cost_indices = ibuffer;

   int num_units = units.size();

   float *cost         = fbuffer;
   float *local_buffer = fbuffer + size[0] * size[1] * size[2];

   // Modify the destination until it selects a valid location or surpases the range
   for (int ind = 0; ind <= Map->get_map_layer () && Map->test_los (destination); ind++)
   {
      if (Map->get_ground_cell (destination)) break;

      destination[2]--;
   }

   // Create the selected cost indices equal to the number of units
   cost_function2 (
         ground_map,
         weight_map,
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

      if (!unit->is_selected() && selected_units) continue;

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

      if (location_value >= 0)
      {
         // Return any jobs the unit has if any
         while (unit->num_return_jobs () > 0) {
            Job *job = unit->pop_return_job ();
std::cout << __FILE__ << __LINE__ << ":returning job " << job << " to the queued jobs" << std::endl;
            queued_jobs.push_front (job);
         }

         // Set the unit destination
         unit->set_destination (dest);
      }
   }
}

void Society::update (float time_step)
{
   const bool  *ground_map = Map->access_ground ();
   const float *weight     = Map->access_weight ();

   float *cost         = fbuffer;
   float *local_buffer = fbuffer + size[0] * size[1] * size[2];

   int iteration_max = 5 + rand () % 10;

   // Assign jobs for units with available job slots
   for (int iteration = 0; iteration < iteration_max; iteration++)
   {
      // Assign ready jobs for the current available unit
      Unit *unit = units.access (current_job_unit_index);

      int unit_location[3] = {
         (int)unit->get_position (0),
         (int)unit->get_position (1),
         (int)unit->get_position (2) };

      if (unit->available_job_slots ())
      {
         if (queued_jobs.size() > 0)
         {
            Job *job = queued_jobs.access (current_job_index);

            // Test if this job has a ground-accessible cell near it by
            // first getting the job's cell location
            int job_location_cell[3] = {
               job->get_position (0),
               job->get_position (1),
               job->get_position (2) };

            // test if this cell is accessible to the unit
            bool accessible =
               cost_function (
                     ground_map,
                     weight,
                     cost,
                     size,
                     job_location_cell,
                     unit_location,
                     local_buffer);

            if (accessible)
            {
std::cout << __FILE__ << __LINE__ << ":assigning job " << job << " to unit " << unit << " num queued jobs = " << queued_jobs.size () << std::endl;
               // The job is accessible to the unit, assign it to the unit
               // and remove this job from the list since it now belongs to the unit
               unit->assign_job (queued_jobs.pop (current_job_index--));
               if (current_job_index < 0) current_job_index = 0;
            }
         }
      }

      // Advance the current unit index (for assigning jobs)
      if (++current_job_unit_index >= units.size ())
      {
         current_job_unit_index = 0;

         // Advance the current job index
         if (++current_job_index >= queued_jobs.size ())
            current_job_index = 0;
      }
   }

   // Update units
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      // Perform the completed job action by this unit and dismiss it
      if (unit->num_jobs () > 0)
      {
         Job *job = unit->access_active_job ();

         int flat_ind = job->get_flattened_loc_index ();

         bool unit_coincides = false;

         // Check if there are any units at this index
         for (int unit_ind2 = 0; unit_ind2 < units.size (); unit_ind2++)
         {
            Unit *unit = units.access (unit_ind2);
            int unit_location[3] = {
               (int)unit->get_position (0),
               (int)unit->get_position (1),
               (int)unit->get_position (2) };

            int unit_flat_ind = dim_to_flat_ind (3, unit_location, size);

            if (unit_flat_ind == flat_ind) unit_coincides = true;
         }

         // Complete the job if it's done and no other units conflict with its completion
         if (job->is_complete () && !unit_coincides)
         {
            // Perform the complete action for the job
            if (job->get_type () == jid::REMOVE)
            {
               unit->add_item (Map->get_material (flat_ind));
               Map->remove_cell (flat_ind);
            }

            if (job->get_type () == jid::BUILD)
            {
               // TODO: unit->remove_item (...)
               Map->add_cell (job->get_material (), flat_ind);
            }

            // Remove the job from the unit's list
            unit->pop_active_job ();

std::cout << __FILE__ << __LINE__ << "unit " << unit << " about to delete job " << job << " num jobs = " << unit->num_jobs () << std::endl;
            delete job;
         }
      }

      // Return all undoable jobs from this unit
      while (unit->num_return_jobs () > 0)
      {
//bool duplicate = queued_jobs.test_duplicates ();
//if (duplicate) std::cout << __FILE__ << __LINE__ << ":duplicate found here" << std::endl;
         Job *job = unit->return_job ();
std::cout << __FILE__ << __LINE__ << ":returning unit " << unit << "'s job " << job << ". num queued jobs = " << queued_jobs.size() << std::endl;
         queued_jobs.push_front (job);
//duplicate = queued_jobs.test_duplicates ();
//if (duplicate) std::cout << __FILE__ << __LINE__ << ":duplicate found here" << std::endl;
      }

      // Update the unit's position and path planning
      unit->update (time_step);
   }

   Map->update ();
}

const bool *Society::access_air ()
{
   return static_cast<const bool*>(Map->access_air ());
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

   int min_x = cell_selections[0][0];
   int max_x = cell_selections[1][0];

   if (max_x < min_x) {
      int temp = min_x;
      min_x = max_x;
      max_x = temp;
   }

   int min_y = cell_selections[0][1];
   int max_y = cell_selections[1][1];

   if (max_y < min_y) {
      int temp = min_y;
      min_y = max_y;
      max_y = temp;
   }

   int min_z = cell_selections[0][2];
   int max_z = cell_selections[1][2];

   if (max_z < min_z) {
      int temp = min_z;
      min_z = max_z;
      max_z = temp;
   }

   max_z += 1.0f;

   int bounding_box[2][3] = {
      { min_x, min_y, min_z },
      { max_x, max_y, max_z } };

   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      unit->select (bounding_box);
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

void Society::select_cells (int cell_selections[2][3])
{
   Map->ready_uncommited_job_cells (cell_selections);
}

void Society::set_select_cells (bool reset_uncommitted_jobs_size, int mode)
{
   Map->set_uncommited_job_cells (reset_uncommitted_jobs_size, mode);
}

void Society::set_jobs (int job_type, unsigned int jobmaterial)
{
   const bool *new_job_location_cells =
      Map->access_uncommitted_jobs ();

   for (int indz = 0, ind = 0; indz < size[2]; indz++)
   {
      for (int indy = 0; indy < size[1]; indy++)
      {
         for (int indx = 0; indx < size[0]; indx++, ind++)
         {

            // Continue if this is not a candidate for a new job
            if (new_job_location_cells[ind] == false) continue;

            bool duplicate = false;

            // Test if this uncommitted job is already in the committed-jobs list
            for (int job_ind = 0; job_ind < queued_jobs.size (); job_ind++) {
               Job *job = queued_jobs.access (job_ind);
               int location_ind = job->get_flattened_loc_index ();
               if (ind == location_ind)
               {
                  duplicate = true;
                  break;
               }
            }

            // If the candidate job is already in the list, continue to the next one
            if (duplicate) continue;

            // Test if this is already assigned to one of the units
            for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
            {
               Unit *unit = units.access (unit_ind);
               for (int job_ind = 0; job_ind < unit->num_total_jobs(); job_ind++)
               {
                  Job *job = unit->access_job (job_ind);
                  int location_ind = job->get_flattened_loc_index ();
                  if (ind == location_ind) duplicate = true;
               }
            }

            if (duplicate) continue;

            int location[3] = {
               flat_ind_to_dim (0, ind, size),
               flat_ind_to_dim (1, ind, size),
               flat_ind_to_dim (2, ind, size) };

            if (job_type == jid::REMOVE) {
               Job *job = new JobRemove (ind, location, Map->get_material (location));
std::cout << __FILE__ << __LINE__ << "creating new REMOVE job " << job << " to append to the jobs list. num queued jobs = " << queued_jobs.size () << std::endl;
               queued_jobs.push_front (job);
            }
            else if (job_type == jid::BUILD) {
               Job *job = new JobBuild (ind, location, jobmaterial);
std::cout << __FILE__ << __LINE__ << "creating new BUILD job " << job << " to append to the jobs list. num queued jobs = " << queued_jobs.size () << std::endl;
               queued_jobs.push_front (job);
            }
            else
               std::cout << __FILE__ << __LINE__ << ":job ID not supported, resolve this" << std::endl;
         }
      }
   }

   Map->unselect_uncommitted_jobs ();
}
