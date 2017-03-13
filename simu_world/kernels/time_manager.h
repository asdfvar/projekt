#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <time.h>
#include <sys/time.h>
#include <cmath>

class Time_manager
{

   public:

      Time_manager( double time_step_desired );
      ~Time_manager( void );

      void wait_for_time( void );
      double get_time_step_actual( void );
      double get_time_step_actual_factor( void );

   private:

      double time_step_desired;
      bool   init_done;
      double start_time;
      double end_time;
      double time_step_actual;
      double time_step_actual_factor;
};

#endif
