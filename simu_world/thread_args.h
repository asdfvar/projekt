#ifndef THREAD_ARGS_H
#define THREAD_ARGS_H

#include "simu_world_obj.h"

typedef struct ARGS
{
   Simu_world_obj*    simu_world_obj;
   pthread_barrier_t* IO_barrier;
} ARGS;

#endif
