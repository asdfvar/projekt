#include <cmath>
#define MAX_COST 999999999.0f

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
   int j = (ind / I) % (I * J);
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

   for (int ind = 0; ind < dim_prod; ind++) cost[ind] = MAX_COST;

   // list of indices of path cost to compute the path cost
   int *path_cost_ind = (int*)(buffer);

   // -1 = infinit cost

   int index;

   index = ijk_to_ind (src[0], src[1], src[2], dim[0], dim[1], dim[2]);

   cost[index] = 0;

   path_cost_ind[0] = index;
   int path_cost_ind_size = 1;

   // loop; for each path cost index
   while (path_cost_ind_size > 0)
   {
      int last_index = path_cost_ind[path_cost_ind_size - 1];

      path_cost_ind_size--;

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

               if (nodes[local_index] < 0) {
                  cost[local_index] = -1;
                  continue;
               }

               float local_cost = nodes[local_index];

               local_cost +=
                  sqrtf((float)((k - sub_k) * (k - sub_k) +
                        (j - sub_j) * (j - sub_j) +
                        (i - sub_i) * (i - sub_i)));

               local_cost += cost[last_index];

               if (local_cost < cost[local_index])
               {
                  cost[local_index] = local_cost;

                  path_cost_ind[path_cost_ind_size] = local_index;
                  path_cost_ind_size++;

                  if (i == dst[0] && j == dst[1] && k == dst[2]) return;
               }
            }
         }
      }
   }
}

void pathfinding (float *cost, int dim[3], int dst[3], int *path)
{

   int I = dim[0];
   int J = dim[1];
   int K = dim[2];

   for (int k = 0; k < K; k++)
   {
      for (int j = 0; j < J; j++)
      {
         for (int i = 0; i < I; i++)
         {
         }
      }
   }
}
