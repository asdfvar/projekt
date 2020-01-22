#include "map.h"
#include <cstdlib>
#include <iostream>

#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))

MAP::MAP (int num_cells[3])
{
   size[0] = num_cells[0];
   size[1] = num_cells[1];
   size[2] = num_cells[2];

   map      = new float[size[0] * size[1] * size[2]];
   ground   = new float[size[0] * size[1] * size[2]];
   material = new int  [size[0] * size[1] * size[2]];

   uncommitted_actions      = new int[size[0] * size[1] * size[2]];
   uncommitted_actions_size = 0;

   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++) map[ind] = 1.0f;

   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            material[ind] = 1;
            if (rand() & 3 && ind_z == 20) material[ind] = 0;
            if (ind_z == 22) material[ind] = 0;
            if (ind_z == 18) material[ind] = 0;
            if (ind_x == ind_z && ind_y == 0) material[ind] = 0;
            if (ind_x == ind_z + 1 && ind_y == 0) material[ind] = 1;

            if (ind_z == 18 &&
                  ind_x > 10 && ind_x < 20 &&
                  ind_y > 10 && ind_y < 20)
               material[ind] = 1;
         }
      }
   }

   // TODO: make this "set_air"
   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++)
   {
      if (material[ind] > 0) map[ind] = -1.0f;
      else map[ind] = 0.0f;
   }

   set_ground ();
}

MAP::~MAP (void)
{
   delete[] map;
   delete[] ground;
   delete[] material;
   delete[] uncommitted_actions;
}

float MAP::get_ground_cell (int flattened_ind)
{
   return ground[flattened_ind];
}

float MAP::get_ground_cell (int ind[3])
{
   int flattened_ind =
      ind[2] * size[0] * size[1] +
      ind[1] * size[0]           +
      ind[0];

   return ground[flattened_ind];
}

// Ground map is dependent on the map. It is the same with the exception that
// open spaces not supported by ground are set as invalid (ground)
void MAP::set_ground (void)
{
   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            // Set the ground value to the map value if there is ground below this cell
            if (ind_z == 0) ground[ind] = map[ind];
            else if (map[ind - size[0] * size[1]] < 0) ground[ind] = map[ind];

            // Default is -1
            else ground[ind] = -1.0f;
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
   map[flattened_cell_index] = value;

   int next_z_ind = flattened_cell_index + size[0] * size[1];

   if (map[flattened_cell_index] < 0) ground[next_z_ind] = map[next_z_ind];
}

// This will prepare cells as "ready" but not committed to be promoted to an action
void MAP::ready_actions (int cell_selections[2][3], bool control_down)
{
   if (!control_down) uncommitted_actions_size = 0;

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
            int action_index =
               ind_x                     +
               ind_y * size[0]           +
               ind_z * size[0] * size[1];

            bool is_action = false;

            // Search the uncommitted actions to see if it's already selected
            for (int action = 0; action < uncommitted_actions_size; action++)
               is_action |= uncommitted_actions[action] == action_index;

            if (is_action) continue;

            // Assign the uncommitted actions
            if (material[action_index] > 0)
               uncommitted_actions[uncommitted_actions_size++] = action_index;
         }
      }
   }
}

void MAP::unselect_uncommitted_actions (void)
{
   uncommitted_actions_size = 0;
}

const int *MAP::access_uncommitted_actions (int *size)
{
   *size = uncommitted_actions_size;
   return (const int*)uncommitted_actions;
}
