#ifndef PATHFINDING_H
#define PATHFINDING_H

int cost_function (float *nodes, float *cost, int dim[3], int src[3], int dst[3], float *buffer);

int pathfinding (float *cost, int dim[3], int src[3], int dst[3], int *path);

#endif
