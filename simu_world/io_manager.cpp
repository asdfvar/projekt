#include "io_manager.h"
#include "thread_args.h"
#include <iostream>

/*
** function name: io_manager
*/
void *io_manager( void *args_in )
{

   ARGS *args = (ARGS*)args_in;
   Simu_world_obj *simu_world_obj = args->simu_world_obj;

   while( true )
   {
      simu_world_obj->update_map();
   }
}
