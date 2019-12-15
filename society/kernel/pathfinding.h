#ifndef PATHFINDING_H
#define PATHFINDING_H

int cost_function_one_step (
      const float *nodes,
      float       *cost,
      int          dim[3],
      int          end[3],
      int         *path_cost_ind,
      int          path_cost_ind_size);

bool cost_function (
      const float *nodes,
      float       *cost,
      int          dim[3],
      int          src[3],
      int          dst[3],
      float       *buffer);

bool cost_function2 (
      const float *nodes,
      float       *cost,
      int         *cost_indices,
      int          dim[3],
      int          start[3],
      int          num_cells,
      float       *buffer);

int pathfinding (
      float *cost,
      int    dim[3],
      int    src[3],
      int    dst[3],
      int   *path);

#endif
