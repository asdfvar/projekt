#include "map.h"
#include "math_utils.h"
#include "math_utils.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))

MAP::MAP (int size_in[3])
{
   size[0] = size_in[0];
   size[1] = size_in[1];
   size[2] = size_in[2];

   cells = new Lattice<Cell> (size, 3);

   map_layer = 0;

   weight     = new float[size[0] * size[1] * size[2]];
   air        = new bool [size[0] * size[1] * size[2]];
   ground     = new bool [size[0] * size[1] * size[2]];
   material   = new int  [size[0] * size[1] * size[2]];
   view_plain = new int  [size[0] * size[1]];
   view_depth = new int  [size[0] * size[1]];

   uncommitted_jobs = new bool[size[0] * size[1] * size[2]];

   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++) air[ind]        = false;
   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++) ground[ind]     = false;
   for (int ind = 0; ind < size[0] * size[1]; ind++)           view_plain[ind] = 0;
   for (int ind = 0; ind < size[0] * size[1]; ind++)           view_depth[ind] = 0;
   for (int ind = 0; ind < size[0] * size[1]; ind++)           weight[ind]     = 0.0f;

   float *perlin_array = new float[size[0] * size[1]];
   int num_grid_cells[2] = { 4, 4 };

   for (int ind = 0; ind < size[0] * size[1]; ind++) perlin_array[ind] = (float)(size[2] / 2);

   perlin (
         perlin_array,
         size,
         4.0f,
         num_grid_cells);

   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0, ind2d = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++, ind2d++)
         {
            if (ind_z < perlin_array[ind2d]) material[ind] = 1;
            else material[ind] = 0;
         }
      }
   }

   delete[] perlin_array;

   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++)
   {
      if (material[ind] > 0) air[ind] = false;
      else air[ind] = true;
   }

   set_map ();

   set_ground ();
}

MAP::~MAP (void)
{
   delete[] weight;
   delete[] air;
   delete[] ground;
   delete[] material;
   delete[] uncommitted_jobs;
   delete[] view_plain;
   delete[] view_depth;
}

void MAP::update (void)
{
   for (int ind = 0; ind < size[0] * size[1]; ind++)
   {
      view_plain[ind] = 0;
      view_depth[ind] = 0;

      // set the view plain to the level of where the terrain is
      for (int depth = 0; depth < 20; depth++)
      {
         int mod_map_layer = map_layer - depth;
         if (mod_map_layer < 0) mod_map_layer = 0;
         int layer_ind = size[0] * size[1] * mod_map_layer + ind;
         if (material[layer_ind] != 0)
         {
            view_plain[ind] = material[layer_ind];
            view_depth[ind] = depth;
            break;
         }
         else if (material[layer_ind] == 0 && mod_map_layer == 0)
         {
            view_plain[ind] = material[layer_ind];
            view_depth[ind] = 1;
            break;
         }
      }
   }

   set_map ();

   set_ground ();
}

bool MAP::get_air_cell (int flattened_ind)
{
   return air[flattened_ind];
}

bool MAP::get_air_cell (int ind[3])
{
   int flattened_ind =
      ind[2] * size[0] * size[1] +
      ind[1] * size[0]           +
      ind[0];

   return air[flattened_ind];
}

bool MAP::get_ground_cell (int flattened_ind)
{
   return ground[flattened_ind];
}

bool MAP::get_ground_cell (int ind[3])
{
   int flattened_ind =
      ind[2] * size[0] * size[1] +
      ind[1] * size[0]           +
      ind[0];

   return ground[flattened_ind];
}

// Ground map is dependent on the air map. It is the same with the exception that
// open spaces not supported by ground are set as invalid (ground)
void MAP::set_map (void)
{
   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++)
   {
      if (material[ind] == 0) air[ind] = true;
      else air[ind] = false;
   }
}

void MAP::set_ground (void)
{
   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            // Default is not ground accessible
            ground[ind] = false;

            // Set the ground value to the map value if there is ground below this cell
            if (ind_z == 0) ground[ind] = air[ind];
            else if (air[ind - size[0] * size[1]] == false) ground[ind] = air[ind];
         }
      }
   }
}

void MAP::change (int flattened_cell_index, float value)
{
   int flattened_index = flattened_cell_index;

   local_change (flattened_index, value);
}

void MAP::change (int cell[3], float value)
{
   int flattened_index =
      cell[0] + 
      cell[1] * size[0] +
      cell[2] * size[0] * size[1];

   local_change (flattened_index, value);
}

void MAP::local_change (int flattened_cell_index, float value)
{
   air[flattened_cell_index] = value;

   int next_z_ind = flattened_cell_index + size[0] * size[1];

   if (air[flattened_cell_index] == false) ground[next_z_ind] = air[next_z_ind];
}

// This will prepare cells as "ready" but not committed to be promoted to a job
void MAP::ready_uncommited_job_cells (int cell_selections_in[2][3])
{
   cell_selections[0][0] = cell_selections_in[0][0];
   cell_selections[0][1] = cell_selections_in[0][1];
   cell_selections[0][2] = cell_selections_in[0][2];

   cell_selections[1][0] = cell_selections_in[1][0];
   cell_selections[1][1] = cell_selections_in[1][1];
   cell_selections[1][2] = cell_selections_in[1][2];
}

void MAP::set_uncommited_job_cells (bool reset_uncommitted_jobs)
{
   if (reset_uncommitted_jobs) unselect_uncommitted_jobs ();

   int start[3] = {
      MIN (cell_selections[0][0], cell_selections[1][0]),
      MIN (cell_selections[0][1], cell_selections[1][1]),
      MIN (cell_selections[0][2], cell_selections[1][2]) };

   int end[3] = {
      MAX (cell_selections[0][0], cell_selections[1][0]),
      MAX (cell_selections[0][1], cell_selections[1][1]),
      MAX (cell_selections[0][2], cell_selections[1][2]) };

   for (int ind_z = start[2]; ind_z <= end[2]; ind_z++) {
      for (int ind_y = start[1]; ind_y < end[1]; ind_y++) {
         for (int ind_x = start[0]; ind_x < end[0]; ind_x++)
         {
            int job_index =
               ind_x                     +
               ind_y * size[0]           +
               ind_z * size[0] * size[1];

            // Assign the uncommitted jobs if there's material
            if (material[job_index] > 0)
               uncommitted_jobs[job_index] = true;
         }
      }
   }
}

void MAP::unselect_uncommitted_jobs (void)
{
   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++)
      uncommitted_jobs[ind] = false;
}

const bool *MAP::access_uncommitted_jobs (void)
{
   return (const bool*)uncommitted_jobs;
}

int MAP::get_view_plain (int ind[2])
{
   int flat_ind = ind[1] * size[0] + ind[0];
   return view_plain[flat_ind];
}

void MAP::remove_cell (int flat_ind)
{
   material[flat_ind] = 0;
   update ();
}
