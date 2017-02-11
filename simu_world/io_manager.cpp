#include "io_manager.h"
#include "thread_args.h"
#include <iostream>

/*
** function name: io_manager
*/
void *io_manager( void *args_in )
{

   ARGS *args = (ARGS*)args_in;

   Simu_world_obj*    simu_world_obj = args->simu_world_obj;
   pthread_barrier_t* IO_barrier     = args->IO_barrier;

//   pthread_barrier_wait( IO_barrier );

   while( true )
   {
      simu_world_obj->update_map();
   }
}
