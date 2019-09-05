#include <iostream>
#include <cmath>
#include <cfloat>

static inline int ijk_to_ind (int i, int j, int k, int I, int J, int K)
{
   int ind = (k * I * J) + (j * I) + i;
   return ind;
}

static inline int ind_to_i (int ind, int I, int J, int K)
{
   int i = ind % I;
   return i;
}

static inline int ind_to_j (int ind, int I, int J, int K)
{
   int j = (ind / I) % J;
   return j;
}

static inline int ind_to_k (int ind, int I, int J, int K)
{
   int k = ind / (I * J);
   return k;
}

void cost_function (float *nodes, float *cost, int dim[3], int src[3], int dst[3], float *buffer)
{
   int dim_prod = dim[0] * dim[1] * dim[2];

   int I = dim[0];
   int J = dim[1];
   int K = dim[2];

   for (int ind = 0; ind < dim_prod; ind++) cost[ind] = FLT_MAX;

   // list of indices of path cost to compute the path cost
   int *path_cost_ind = (int*)(buffer);

   // -1 = infinit cost

   int index;

   index = ijk_to_ind (src[0], src[1], src[2], dim[0], dim[1], dim[2]);

   cost[index] = 0.0f;

   path_cost_ind[0] = index;
   int path_cost_ind_size = 1;

   // loop; for each path cost index
   while (path_cost_ind_size > 0)
   {
      int last_index = path_cost_ind[--path_cost_ind_size];

      int sub_i = ind_to_i (last_index, I, J, K);
      int sub_j = ind_to_j (last_index, I, J, K);
      int sub_k = ind_to_k (last_index, I, J, K);

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

               if (nodes[local_index] < 0.0f) {
                  cost[local_index] = -1;
                  continue;
               }

               float local_cost = cost[last_index];

               int i_dist = (i - sub_i) * (i - sub_i);
               int j_dist = (j - sub_j) * (j - sub_j);
               int k_dist = (k - sub_k) * (k - sub_k);

               /*
               ** sqrtf((float)((k - sub_k) * (k - sub_k) +
               **       (j - sub_j) * (j - sub_j) +
               **       (i - sub_i) * (i - sub_i)));
               */
               local_cost +=
                  i_dist && j_dist && k_dist ? 1.732050807569f :
                  j_dist && k_dist ? 1.414213562373f :
                  i_dist && j_dist ? 1.414213562373f :
                  i_dist && k_dist ? 1.414213562373f :
                  i_dist    ? 1.0f :
                  j_dist    ? 1.0f :
                  k_dist    ? 1.0f : 0.0f;

               local_cost += nodes[local_index];

               if (local_cost < cost[local_index])
               {
                  cost[local_index] = local_cost;
                  path_cost_ind[path_cost_ind_size++] = local_index;

                  if (dst != nullptr && i == dst[0] && j == dst[1] && k == dst[2]) return;
               }
            }
         }
      }
   }
}

int pathfinding (float *cost, int dim[3], int src[3], int dst[3], int *path)
{

   int I = dim[0];
   int J = dim[1];
   int K = dim[2];

   int dim_prod = I * J * K;

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

               if (cost[local_index] < 0.0f) continue;

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
