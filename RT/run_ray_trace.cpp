#include "ray_trace.h"
#include <pthread.h>
#include <iostream>

#define MAX_THREADS 20

/*
** Declare local functions and structs
*/
struct ARGS
{
   unsigned int thread_id;
   unsigned int num_threads;
   RayTrace *RT;
};

void *sub_run_wrapper ( void* );

void sub_run ( unsigned int thread_id,
               unsigned int num_threads,
               RayTrace *RT);

/*
** Function NAME: sub_run
**
** Main body of algorithms
*/
void sub_run ( unsigned int thread_id,
               unsigned int num_threads,
               RayTrace *RT)
{

   int nxy = RT->get_nxy();
   int num_points;

   if (thread_id == num_threads - 1)
   {
      num_points = nxy / num_threads + nxy % num_threads;
   }
   else
   {
      num_points = nxy / num_threads;
   }

   Ray *grid_alias = RT->grid + nxy / num_threads * thread_id;

}

/*
** Function NAME: sub_run_wrapper
*/
void *sub_run_wrapper ( void *args_in)
{
   struct ARGS *args = (ARGS*)args_in;

   sub_run (args->thread_id,
            args->num_threads,
            args->RT);

   return NULL;
}

/*
** Function NAME: run
*/
void RayTrace::run ( unsigned int num_threads )
{

   struct ARGS args[MAX_THREADS];

   pthread_t threads[MAX_THREADS];
   // pthread_attr_t attr;
   // void *status;

   /*
   ** kick off threads
   */
   for (unsigned int id = 0; id < num_threads; id++)
   {

      args[id].thread_id   = id;
      args[id].num_threads = num_threads;
      args[id].RT          = this;

      std::cout << "creating thread " << id << std::endl;

      int stat = pthread_create (&threads[id], NULL, sub_run_wrapper, (void*)&args[id]);

      if (stat != 0)
      {
         std::cout << "Error creating thread" << std::endl;
      }
   }

std::cout << "waiting for threads to finish" << std::endl;
   /*
   ** Wait for threads to finish
   */
   for (unsigned int id = 0; id < num_threads; id++)
   {
      pthread_join(threads[id], NULL); /* Wait until thread is finished */
   }
std::cout << "threads finished" << std::endl;

   pthread_exit (NULL);

}
