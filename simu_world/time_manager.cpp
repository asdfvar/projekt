#include "time_manager.h"
#include <iostream>
#include <cmath>

Time_manager::Time_manager( double time_step_in)
{
   time_step = time_step_in;
   init_done = false;
}

Time_manager::~Time_manager( void)
{
}

void Time_manager::wait_for_time( void )
{
   struct timeval start, end;

   if (!init_done)
   {
      gettimeofday(&start, NULL);
      init_done = true;
   }

   double dt;

   do {
      gettimeofday(&end, NULL);
      end_time = (end.tv_sec*1000000 + end.tv_usec) / 1000000.0;

      dt = end_time - start_time;

   } while (dt < time_step);

   time_step_actual = floor(dt / time_step) * time_step;

   gettimeofday(&start, NULL);
   start_time = (start.tv_sec*1000000 + start.tv_usec) / 1000000.0;
}

float Time_manager::get_time_step( void)
{
   return time_step_actual;
}
