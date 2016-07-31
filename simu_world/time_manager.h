#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <time.h>
#include <sys/time.h>
#include <cmath>

class Time_manager
{

   public:

      Time_manager( double time_step);
      ~Time_manager( void);

      void wait_for_time( void);
      float get_time_step( void);

   private:

      double time_step;
      bool   init_done;
      double start_time;
      double end_time;
      double time_step_actual;
};

#endif
