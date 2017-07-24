#ifndef THREAD_ARGS_H
#define THREAD_ARGS_H

#include "simu_world_obj.h"

typedef struct ARGS
{
   Simu_world_obj*    simu_world_obj;
   pthread_barrier_t* IO_barrier;
   pthread_barrier_t* update_barrier;
   pthread_mutex_t*   IO_mutex;
} ARGS;

#endif
