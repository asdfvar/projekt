#include "ray_trace.h"
#include <pthread.h>
#include <iostream>

#define MAX_THREADS 20

struct ARGS
{
   Ray *grid;
   unsigned int num_points;
};



void *sub_run_wrapper ( void* );

void sub_run (
               Ray *grid,
               unsigned int num_points);


/*
** Function NAME: sub_run
*/
void sub_run (
               Ray *grid,
               unsigned int num_points)
{
   // run on the specified grid
   std::cout << "output from thread" << std::endl;
}

/*
** Function NAME: sub_run_wrapper
*/
void *sub_run_wrapper ( void *args_in)
{
   struct ARGS *args = (ARGS*)args_in;

   sub_run (args->grid,
            args->num_points);

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
      args[id].grid = grid;
      args[id].num_points = 10; // function of num_threads

      std::cout << "creating thread " << id << std::endl;
      int stat = pthread_create (&threads[id], NULL, sub_run_wrapper, (void*)&args[id]);
      if (stat == 0) std::cout << "stat = " << stat << std::endl;
   }

   /*
   ** Wait for threads to finish
   */
   for (unsigned int id = 0; id < num_threads; id++)
   {
      pthread_join(threads[id], NULL); /* Wait until thread is finished */
   }

   pthread_exit (NULL);

}
