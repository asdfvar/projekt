#ifndef PATHFINDING_H
#define PATHFINDING_H

inline int ind_to_i (int ind, int I, int J, int K)
{
   int i = ind % I;
   return i;
}

inline int ind_to_j (int ind, int I, int J, int K)
{
   int j = (ind / I) % J;
   return j;
}

inline int ind_to_k (int ind, int I, int J, int K)
{
   int k = ind / (I * J);
   return k;
}

int search (
      const bool *nodes,
      int         dim[3],
      int         src[3]);

int cost_function_one_step (
      const bool  *nodes,
      const float *weight,
      float       *cost,
      int          dim[3],
      int          end[3],
      int         *path_cost_ind,
      int          path_cost_ind_size);

bool cost_function (
      const bool  *nodes,
      const float *weight,
      float       *cost,
      int          dim[3],
      int          src[3],
      int          dst[3],
      float       *buffer);

bool cost_function2 (
      const bool  *nodes,
      const float *weight,
      float       *cost,
      int         *cost_indices,
      int          dim[3],
      int          start[3],
      int          num_cells,
      float       *buffer);

int pathfinding (
      const bool *nodes,
      float      *cost,
      int         dim[3],
      int         src[3],
      int         dst[3],
      int        *path);

#endif
