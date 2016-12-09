#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

class Task_manager
{
   public:

           Task_manager( unsigned int num_tasks);
          ~Task_manager( void);
      bool task_enabled( int task);
      int  enable_task( int task);
      int  disable_task( int task);
      void execute_commands( void);

      void print_status( void);

   private:

      unsigned int  num_tasks;
      bool         *tasks;
      bool         *tasks_to_enable;
      bool         *tasks_to_disable;

};

#endif
