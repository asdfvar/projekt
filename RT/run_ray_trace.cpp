#include "ray_trace.h"
#include "debug_rt.h"
#include <pthread.h>
#include <iostream>
#include "linalg.h"
#include <list>

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
** Function NAME: execute
*/
void RayTrace::execute (unsigned int thread_id,
                        unsigned int num_threads)
{

   int   num_rays;
   float direction[3];

   int nxy = nx * ny;

   if (thread_id == num_threads - 1)
   {
      num_rays = nxy / num_threads + nxy % num_threads;
   }
   else
   {
      num_rays = nxy / num_threads;
   }

   Ray *grid_alias = grid + nxy / num_threads * thread_id;

   for (int k = 0; k < num_rays; k++)
   {

      Ray output_ray;
      float distance;
      bool intersect = intersect_objects (
                            grid_alias[k],
                           &output_ray,
                           &distance);

#ifdef DEBUG_1
      if (k % 10 == 0) std::cout << std::endl;
      if (intersect)
      {
//      std::cout << "1";
      std::cout << distance << " ";
      }
      else {
      std::cout << " ";
      }
#endif

      if (intersect)
      {
         for (std::list<Light_source> ::iterator light_it = lights.begin();
              light_it != lights.end();
              light_it++)
         {

            // direction to light source = light source position - output ray position
            direction[0] = light_it->get_position(0) - output_ray.get_position(0);
            direction[1] = light_it->get_position(1) - output_ray.get_position(1);
            direction[2] = light_it->get_position(2) - output_ray.get_position(2);

            // angle of reflected ray towards light source cos(th) = a . b / (|a||b|)
            float output_direction[3];

            output_direction[0] = output_ray.get_direction(0);
            output_direction[1] = output_ray.get_direction(1);
            output_direction[2] = output_ray.get_direction(2);

            float num  = linalg::dot_product<float>( output_direction, direction, 3);
            float den  = linalg::norm<float>(output_direction, 3);
                  den *= linalg::norm<float>(direction, 3);
            float costh = num / den;
            float theta = acos(costh);
            float th_range = theta / 3.14159265358979323846264f;

            float score = 1.0f - th_range;
            float intensity[3] = { score, score, score };

            grid_alias[k].increment_intensity( intensity );

         }
      }

   }

}

/*
** Function NAME: sub_run
*/
void sub_run ( unsigned int thread_id,
               unsigned int num_threads,
               RayTrace *RT)
{

   RT->execute (thread_id,
                num_threads);

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

//   pthread_exit (NULL);

}
