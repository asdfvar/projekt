#include "math_constants.h"
#include "interpolate.h"
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
   int nxy = nx * ny;

   float position[3], direction[3], intensity[3];
   float reflectivity;

   if (thread_id == num_threads - 1)
   {
      num_rays = nxy / num_threads + nxy % num_threads;
   }
   else
   {
      num_rays = nxy / num_threads;
   }

   Ray *grid_alias = grid + nxy / num_threads * thread_id;

   float color_intensity[3] = {0.0f, 0.0f, 0.0f};

   for (int iteration = 0; iteration < 20; iteration++)
   {

      /*
      ** Loop through all the rays
      */
      for (int ray_ind = 0; ray_ind < num_rays; ray_ind++)
      {
   
         if ( grid_alias[ray_ind].is_valid() )
         {
   
            float *reflection_table_x;
            float *reflection_table_y;
            int    reflection_table_N;
   
            Ray output_ray;
            float distance;
   
            bool intersect = all_objects.intersect (
                                 grid_alias[ray_ind],
                                 &output_ray,
                                 &distance,
                                  color_intensity,
                                 &reflectivity,
                                 &reflection_table_x,
                                 &reflection_table_y,
                                 &reflection_table_N);
   
            if (intersect)
            {
               for (std::list<Light_source>::iterator light_it = lights.begin();
                    light_it != lights.end();
                    light_it++)
               {
   
                  float light_source_direction[3];
   
                  // direction to light source = light source position - output ray position
                  light_source_direction[0] = light_it->get_position(0) - output_ray.get_position(0);
                  light_source_direction[1] = light_it->get_position(1) - output_ray.get_position(1);
                  light_source_direction[2] = light_it->get_position(2) - output_ray.get_position(2);
   
                  // angle of reflected ray towards light source cos(th) = a . b / (|a||b|)
                  direction[0] = output_ray.get_direction(0);
                  direction[1] = output_ray.get_direction(1);
                  direction[2] = output_ray.get_direction(2);
   
                  position[0] = output_ray.get_position(0);
                  position[1] = output_ray.get_position(1);
                  position[2] = output_ray.get_position(2);
   
                  float theta = linalg::angle_offset<float>(
                                             direction,
                                             light_source_direction,
                                             3);
   
                  float th_range = theta / PI;
   
                  float score = tools::linear_interpolate(
                                             reflection_table_x,
                                             reflection_table_y,
                                             th_range,
                                             reflection_table_N);
   
                  score *= grid_alias[ray_ind].get_energy();
   
                  intensity[0] = score;
                  intensity[1] = score;
                  intensity[2] = score;
   
                  intensity[0] *= light_it->get_intensity(0);
                  intensity[1] *= light_it->get_intensity(1);
                  intensity[2] *= light_it->get_intensity(2);
   
                  intensity[0] *= color_intensity[0];
                  intensity[1] *= color_intensity[1];
                  intensity[2] *= color_intensity[2];
   
                  grid_alias[ray_ind].increment_intensity( intensity, reflectivity);
   
                  grid_alias[ray_ind].set_position( position );
                  grid_alias[ray_ind].set_direction( direction );

               }
            }
            else
            {
               grid_alias[ray_ind].set_invalid();
            }
         }
      }
   }

   for (int ray_ind = 0; ray_ind < num_rays; ray_ind++)
   {
      intensity[0] = grid_alias[ray_ind].get_intensity(0);
      intensity[1] = grid_alias[ray_ind].get_intensity(1);
      intensity[2] = grid_alias[ray_ind].get_intensity(2);


      grid_alias[ray_ind].set_intensity( intensity );
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
