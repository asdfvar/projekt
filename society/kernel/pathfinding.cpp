#include "pathfinding.h"
#include <iostream>
#include <cmath>
#include <cfloat>

// #define USE_GENERAL
#define USE_EUCLIDEAN

#define SEARCH_RADIUS 10

static inline int ijk_to_ind (int i, int j, int k, int I, int J, int K)
{
   int ind = (k * I * J) + (j * I) + i;
   return ind;
}

static inline int ind_to_axis (int index, int *dim, int select_axis_ind)
{
   int denomitator = 1;
   for (int ind = 0; ind < select_axis_ind; ind++) denomitator *= dim[ind];
   
   return (index / denomitator) % dim[select_axis_ind];
}

int search (
      const bool *nodes,
      int         dim[3],
      int         src[3])
{
   bool found = false;

   int mod_ind;

   enum directions {
      UP,
      LEFT,
      FORWARD,
      RIGHT,
      BACK,
      DOWN,
      SIZE };

   int perm[SEARCH_RADIUS];

   for (int radius = 0; !found && radius <= SEARCH_RADIUS; radius++)
   {
      unsigned int combinations = 1;
      for (int ind = 0; ind < radius; ind++) combinations *= 6;

      for (int ind = 0; ind < radius; ind++) perm[ind] = 0;

      for (int itt = 0; itt < combinations; itt++)
      {
         int curr_ind = 
            ijk_to_ind (src[0], src[1], src[2], dim[0], dim[1], dim[2]);

         mod_ind = curr_ind;

         // traverse the current permutation to produce the modified index
         for (int ind = 0; ind < radius; ind++) {
            int x = ind_to_axis (mod_ind, dim, 0);
            int y = ind_to_axis (mod_ind, dim, 1);
            int z = ind_to_axis (mod_ind, dim, 2);

            if      (perm[ind] == UP     ) { z++; if (z >= dim[2]) z = dim[2] - 1; }
            else if (perm[ind] == LEFT   ) { x--; if (x < 0      ) x = 0;          }
            else if (perm[ind] == FORWARD) { y++; if (y >= dim[1]) y = dim[1] - 1; }
            else if (perm[ind] == RIGHT  ) { x++; if (x >= dim[0]) x = dim[0] - 1; }
            else if (perm[ind] == BACK   ) { y--; if (y < 0      ) y = 0;          }
            else if (perm[ind] == DOWN   ) { z--; if (z < 0      ) z = 0;          }

            mod_ind = ijk_to_ind (x, y, z, dim[0], dim[1], dim[2]);
         }

         // Check the current permutation
         if (nodes[mod_ind] == true) {
            found = true;
            break;
         }

         // Increment the permutation
         perm[0]++;
         for (int ind = 0; ind < radius; ind++) {
            if (perm[ind] == SIZE) {
               perm[ind+1]++;
               perm[ind] = 0;
            }
         }
      }
   }
   return mod_ind;
}

int cost_function_one_step (
      const bool  *nodes,
      const float *weight,
      float       *cost,
      int          dim[3],
      int          end[3],
      int         *path_cost_ind,
      int          path_cost_ind_size)
{
   int I = dim[0];
   int J = dim[1];
   int K = dim[2];

#ifdef USE_GENERAL

   // Generalized for any discrete space.
   // This approach simply selects the next available cell.

   int best_index = path_cost_ind[--path_cost_ind_size];

#elif defined(USE_EUCLIDEAN)

   // Specific to the Euclidean metric space. Find the next valid index
   // that is the closest in distance to the objective (ignoring obstacles).
   // This approach biases its selection to the next available cell that is
   // closest to the objective in the standard metric space

   int best_cost_index = 0;
   int best_index = path_cost_ind[0];

   int min_dist = INT32_MAX;
   for (int ind = 0; ind < path_cost_ind_size; ind++)
   {
      int i = ind_to_i (path_cost_ind[ind], I, J, K);
      int j = ind_to_j (path_cost_ind[ind], I, J, K);
      int k = ind_to_k (path_cost_ind[ind], I, J, K);

      // Determine the square of the distance to the objective
      int dist = ((k - end[2]) * (k - end[2]) +
            (j - end[1]) * (j - end[1]) +
            (i - end[0]) * (i - end[0]));

      // Update the best index if it is found
      if (dist < min_dist)
      {
         best_index = path_cost_ind[ind];
         best_cost_index = ind;
         min_dist = dist;
      }
   }

   // Remove the best-cost index from the list
   for (int sub_ind = best_cost_index; sub_ind < path_cost_ind_size - 1; sub_ind++) {
      path_cost_ind[sub_ind] = path_cost_ind[sub_ind + 1];
   }

   path_cost_ind_size--;
#endif

   // For each neighboring cell, update the cost function and append to the list
   // of available cell Indices for the path cost index list
   int sub_i = ind_to_i (best_index, I, J, K);
   int sub_j = ind_to_j (best_index, I, J, K);
   int sub_k = ind_to_k (best_index, I, J, K);

   for (int k = sub_k - 1; k <= sub_k + 1; k++)
   {
      if (k < 0 || k >= K) continue;

      for (int j = sub_j - 1; j <= sub_j + 1; j++)
      {
         if (j < 0 || j >= J) continue;

         for (int i = sub_i - 1; i <= sub_i + 1; i++)
         {
            if (i < 0 || i >= I) continue;

            int local_index = ijk_to_ind (i, j, k, I, J, K);
            if (nodes[local_index] == false) {
               cost[local_index] = 0.0f;
               continue;
            }

            // Logic to ignore diagonals if corresponding adjacent
            // cells are blocked
            if (i == sub_i - 1 && j == sub_j - 1) {
               if (nodes[ijk_to_ind (sub_i, sub_j-1, sub_k, I, J, K)] == false ||
                     nodes[ijk_to_ind (sub_i-1, sub_j, sub_k, I, J, K)] == false) continue;
            }

            if (i == sub_i + 1 && j == sub_j + 1) {
               if (nodes[ijk_to_ind (sub_i, sub_j+1, sub_k, I, J, K)] == false ||
                     nodes[ijk_to_ind (sub_i+1, sub_j, sub_k, I, J, K)] == false) continue;
            }

            if (i == sub_i + 1 && j == sub_j - 1) {
               if (nodes[ijk_to_ind (sub_i, sub_j-1, sub_k, I, J, K)] == false ||
                     nodes[ijk_to_ind (sub_i+1, sub_j, sub_k, I, J, K)] == false) continue;
            }

            if (i == sub_i - 1 && j == sub_j + 1) {
               if (nodes[ijk_to_ind (sub_i, sub_j+1, sub_k, I, J, K)] == false ||
                     nodes[ijk_to_ind (sub_i-1, sub_j, sub_k, I, J, K)] == false) continue;
            }

            float local_cost = cost[best_index];

            /*
             ** sqrtf((float)((k - sub_k) * (k - sub_k) +
             **       (j - sub_j) * (j - sub_j) +
             **       (i - sub_i) * (i - sub_i)));
             */
            int i_dist = (i > sub_i) || (sub_i > i) ? 1 : 0;
            int j_dist = (j > sub_j) || (sub_j > j) ? 1 : 0;
            int k_dist = (k > sub_k) || (sub_k > k) ? 1 : 0;

            local_cost +=
               i_dist && j_dist && k_dist ? 1.732050807569f :
               j_dist && k_dist || i_dist && j_dist || i_dist && k_dist ? 1.414213562373f :
               i_dist || j_dist || k_dist ? 1.0f :
               0.0f;

            local_cost += weight[local_index];

            if (local_cost < cost[local_index])
            {
               cost[local_index] = local_cost;
               path_cost_ind[path_cost_ind_size++] = local_index;

               // Finish if the destination has been found
               if (end != nullptr && i == end[0] && j == end[1] && k == end[2])
               {
                  // Condition if the objective has been found
                  return -1;
               }
            }

         }
      }
   }

   return path_cost_ind_size;
}

// Produce the cost function until the destination is met
bool cost_function (
      const bool  *nodes,
      const float *weight,
      float       *cost,
      int          dim[3],
      int          start[3],
      int          end[3],
      float       *buffer)
{
   const int dim_prod = dim[0] * dim[1] * dim[2];

   const int I = dim[0];
   const int J = dim[1];
   const int K = dim[2];

   for (int ind = 0; ind < dim_prod; ind++) cost[ind] = FLT_MAX;

   // list of indices of path cost to compute the path cost
   int *path_cost_ind = (int*)(buffer);

   int index;

   index = ijk_to_ind (start[0], start[1], start[2], dim[0], dim[1], dim[2]);

   cost[index] = 0.0f;

   // Set the "start" index as the starting index
   path_cost_ind[0] = index;
   int path_cost_ind_size = 1;

   // Build the cost function for all valid spaces available to it
   // or until the end cell has been reached (implicitly determined
   // by the subsequent function call via the "path_cost_ind_size")
   while (path_cost_ind_size > 0)
   {
      path_cost_ind_size = cost_function_one_step (
            nodes,
            weight,
            cost,
            dim,
            end,
            path_cost_ind,
            path_cost_ind_size);
   }

   // Condition if the objective has been found
   if (path_cost_ind_size == -1) return true;

   return false;
}

int pathfinding (
      const bool *nodes,
      float      *cost,
      int         dim[3],
      int         src[3],
      int         dst[3],
      int         *path)
{
   const int I = dim[0];
   const int J = dim[1];
   const int K = dim[2];

   const int dim_prod = I * J * K;

   int current_step = ijk_to_ind (src[0], src[1], src[2], I, J, K);

   int dst_index = ijk_to_ind (dst[0], dst[1], dst[2], I, J, K);

   bool changing = true;

   int path_size = 0;

   while (current_step != dst_index && changing)
   {
      changing = false;

      int sub_i = ind_to_i (current_step, I, J, K);
      int sub_j = ind_to_j (current_step, I, J, K);
      int sub_k = ind_to_k (current_step, I, J, K);

      // Check the nearest neighbors for the next valid path
      for (int k = sub_k - 1; k <= sub_k + 1; k++)
      {
         if (k < 0 || k >= K) continue;
         for (int j = sub_j - 1; j <= sub_j + 1; j++)
         {
            if (j < 0 || j >= J) continue;
            for (int i = sub_i - 1; i <= sub_i + 1; i++)
            {
               if (i < 0 || i >= I) continue;

               int local_index = ijk_to_ind (i, j, k, I, J, K);

               if (nodes[local_index] == false) continue;

               if (cost[local_index] < cost[current_step]) {
                  current_step = local_index;
                  changing = true;
               }
            }
         }
      }

      if (changing) path[path_size++] = current_step;
   }

   return path_size;
}

// Produce the cost function for the number of cells requested
bool cost_function2 (
      const bool  *nodes,
      const float *weight,
      float       *cost,
      int         *cost_indices,
      int          dim[3],
      int          start[3],
      int          num_cells,
      float       *buffer)
{
   const int dim_prod = dim[0] * dim[1] * dim[2];

   const int I = dim[0];
   const int J = dim[1];
   const int K = dim[2];

   for (int ind = 0; ind < dim_prod; ind++) cost[ind] = FLT_MAX;

   // list of indices of path cost to compute the path cost
   int *path_cost_ind = (int*)(buffer);

   // Initialize the cost to zero at the starting index
   int index;
   index = ijk_to_ind (start[0], start[1], start[2], dim[0], dim[1], dim[2]);
   cost[index] = 0.0f;

   // Set the "start" index as the starting index.
   // The path-cost index list contains all cells on the perimeter of the
   // cost function that potentially have new cells adjacent to it
   path_cost_ind[0] = index;
   int path_cost_ind_size = 1;

   cost_indices[0] = index;

   int filled_cells = 1;

   // Build the cost function
   while (filled_cells < num_cells && path_cost_ind_size > 0)
   {
      // pick the path-cost index closest to start
      int min_dist = 999999;
      int select_ind = 0;
      for (int ind = 0; ind < path_cost_ind_size; ind++)
      {
         int i = ind_to_i (path_cost_ind[ind], I, J, K);
         int j = ind_to_j (path_cost_ind[ind], I, J, K);
         int k = ind_to_k (path_cost_ind[ind], I, J, K);

         int dist =
            (i - start[0]) * (i - start[0]) +
            (j - start[1]) * (j - start[1]) +
            (k - start[2]) * (k - start[2]);

         if (dist < min_dist) {
            min_dist = dist;
            select_ind = ind;
         }
      }

      // pop off this index from the list
      int best_index = path_cost_ind[select_ind];
      path_cost_ind_size--;
      for (int ind = select_ind; ind < path_cost_ind_size; ind++) path_cost_ind[ind] = path_cost_ind[ind + 1];

      // For each neighboring cell, update the cost function and append to the list
      // of available cell Indices for the path cost index list
      int sub_i = ind_to_i (best_index, I, J, K);
      int sub_j = ind_to_j (best_index, I, J, K);
      int sub_k = ind_to_k (best_index, I, J, K);

      for (int k = sub_k - 1; k <= sub_k + 1; k++)
      {
         if (filled_cells >= num_cells) break;
         if (k < 0 || k >= K) continue;

         for (int j = sub_j - 1; j <= sub_j + 1; j++)
         {
            if (filled_cells >= num_cells) break;
            if (j < 0 || j >= J) continue;

            for (int i = sub_i - 1; i <= sub_i + 1; i++)
            {
               if (filled_cells >= num_cells) break;
               if (i < 0 || i >= I) continue;

               int local_index = ijk_to_ind (i, j, k, I, J, K);

               if (nodes[local_index] == false) {
                  cost[local_index] = -1.0f;
                  continue;
               }

               // Logic to ignore diagonals if corresponding adjacent
               // cells are blocked
               if (i == sub_i - 1 && j == sub_j - 1) {
                  if (nodes[ijk_to_ind (sub_i, sub_j-1, sub_k, I, J, K)] == false ||
                        nodes[ijk_to_ind (sub_i-1, sub_j, sub_k, I, J, K)] == false) continue;
               }

               if (i == sub_i + 1 && j == sub_j + 1) {
                  if (nodes[ijk_to_ind (sub_i, sub_j+1, sub_k, I, J, K)] == false ||
                        nodes[ijk_to_ind (sub_i+1, sub_j, sub_k, I, J, K)] == false) continue;
               }

               if (i == sub_i + 1 && j == sub_j - 1) {
                  if (nodes[ijk_to_ind (sub_i, sub_j-1, sub_k, I, J, K)] == false ||
                        nodes[ijk_to_ind (sub_i+1, sub_j, sub_k, I, J, K)] == false) continue;
               }

               if (i == sub_i - 1 && j == sub_j + 1) {
                  if (nodes[ijk_to_ind (sub_i, sub_j+1, sub_k, I, J, K)] == false ||
                        nodes[ijk_to_ind (sub_i-1, sub_j, sub_k, I, J, K)] == false) continue;
               }

               // The cost for the new cell is determined as follows:
               // cost_{new\_cell} = cost_{old_cell} + distance + node_{new\_cell}
               float local_cost = cost[best_index];

               /*
                ** sqrtf((float)((k - sub_k) * (k - sub_k) +
                **       (j - sub_j) * (j - sub_j) +
                **       (i - sub_i) * (i - sub_i)));
                */
               int i_dist = (i > sub_i) || (sub_i > i) ? 1 : 0;
               int j_dist = (j > sub_j) || (sub_j > j) ? 1 : 0;
               int k_dist = (k > sub_k) || (sub_k > k) ? 1 : 0;

               local_cost +=
                  i_dist && j_dist && k_dist ? 1.732050807569f :
                  j_dist && k_dist || i_dist && j_dist || i_dist && k_dist ? 1.414213562373f :
                  i_dist || j_dist || k_dist ? 1.0f :
                  0.0f;

               local_cost += weight[local_index];

               if (local_cost < cost[local_index])
               {
                  if (cost[local_index] == FLT_MAX) cost_indices[filled_cells++] = local_index;

                  cost[local_index] = local_cost;
                  path_cost_ind[path_cost_ind_size++] = local_index;
               }
            }
         }
      }
   }

   return true;
}
