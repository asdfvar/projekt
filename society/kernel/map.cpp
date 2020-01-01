#include "map.h"
#include <cstdlib>
#include <iostream>

MAP::MAP (int num_cells[3])
{
   size[0] = num_cells[0];
   size[1] = num_cells[1];
   size[2] = num_cells[2];

   map          = new float[size[0] * size[1] * size[2]];
   ground       = new float[size[0] * size[1] * size[2]];
   action_cells = new int  [size[0] * size[1] * size[2]];

   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++) map[ind] = 1.0f;

   for (int ind_z = 0, ind = 0; ind_z < size[2]; ind_z++) {
      for (int ind_y = 0; ind_y < size[1]; ind_y++) {
         for (int ind_x = 0; ind_x < size[0]; ind_x++, ind++)
         {
            if (rand() & 3 && ind_z == 20) map[ind] = 0.0f;
            else if (ind_z == 22) map[ind] = 0.0f;
            else if (ind_x == ind_z && ind_y == 0) map[ind] = 0.0f;
            else map[ind] = -1.0f;

            action_cells[ind] = 0;
         }
      }
   }

   set_ground ();
}

MAP::~MAP (void)
{
   delete[] map;
   delete[] ground;
   delete[] action_cells;
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

// TODO: define the action cells to be directly representative to the dig action as defined withh that other "todo" item
void MAP::set_dig (float selection_box[2][3])
{

// TODO: remove this conversion logic outta society as a whole
   float partial_col_min = (selection_box[0][0] + 1.0f) / 2.0f * (float)size[0];
   float partial_row_min = (selection_box[0][1] + 1.0f) / 2.0f * (float)size[1];

   float partial_col_max = (selection_box[1][0] + 1.0f) / 2.0f * (float)size[0];
   float partial_row_max = (selection_box[1][1] + 1.0f) / 2.0f * (float)size[1];

   if (partial_col_max < partial_col_min) {
      float temp = partial_col_min;
      partial_col_min = partial_col_max;
      partial_col_max = temp;
   }

   if (partial_row_max < partial_row_min) {
      float temp = partial_row_min;
      partial_row_min = partial_row_max;
      partial_row_max = temp;
   }

   int start[3] = {
      (int)partial_col_min,
      (int)partial_row_min,
      (int)selection_box[0][2] };

   int end[3] = {
      (int)partial_col_max,
      (int)partial_row_max,
      (int)selection_box[1][2] };

std::cout << "start = " << start[0] << ", " << start[1] << ", " << start[2] << std::endl;
std::cout << "end = " << end[0] << ", " << end[1] << ", " << end[2] << std::endl;

   for (int ind = 0; ind < size[0] * size[1] * size[2]; ind++) action_cells[ind] = 0;

   for (int ind_z = start[2]; ind_z <= end[2]; ind_z++) {
      for (int ind_y = start[1]; ind_y < end[1]; ind_y++) {
         for (int ind_x = start[0]; ind_x < end[0]; ind_x++)
         {
            int ind =
               ind_x                     +
               ind_y * size[0]           +
               ind_z * size[0] * size[1];

            action_cells[ind] = 1;
         }
      }
   }
}
