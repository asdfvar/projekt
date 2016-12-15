#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore
{
   public:

      Semaphore( unsigned int num_tasks);
     ~Semaphore( void);

      int  increment_task ( int task);
      int  decrement_task ( int task);
      int  task_pool      ( int task);
      void wait_for_task  ( int task);

      void print_status   ( void    );

   private:

      unsigned int  num_tasks;
      int          *tasks;
      bool         *tasks_to_enable;
      bool         *tasks_to_disable;

};

#endif
