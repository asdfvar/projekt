#include "task_manager.h"
#include <iostream>

Task_manager::Task_manager( unsigned int num_tasks_in)
{
   num_tasks   = num_tasks_in;

   tasks             = new bool[num_tasks];
   tasks_to_enable   = new bool[num_tasks];
   tasks_to_disable  = new bool[num_tasks];

   for (unsigned int task = 0; task < num_tasks; task++)
   {
      tasks[task]            = false;
      tasks_to_enable[task]  = false;
      tasks_to_disable[task] = false;
   }
}

Task_manager::~Task_manager( void)
{
   delete[] tasks;
   delete[] tasks_to_enable;
   delete[] tasks_to_disable;
}

bool Task_manager::task_enabled( int task)
{
   return tasks[task];
}

int Task_manager::enable_task( int task)
{
   if (task > num_tasks)
   {
      std::cout << "task ID exceeds the number of tasks available" << std::endl;
      return -1;
   }
   if (tasks_to_enable[task])
   {
      std::cout << "task "
                << task
                << " already enabled"
                << std::endl;
      return -2;
   }
   else {
      tasks_to_enable[task] = true;
   }

   return 0;
}

int Task_manager::disable_task( int task)
{
   if (task > num_tasks)
   {
      std::cout << "task ID exceeds the number of tasks available" << std::endl;
      return -1;
   }
   else {
      tasks_to_disable[task] = true;
   }
   return 0;
}

void Task_manager::execute_commands( void)
{
   for (int task = 0; task < num_tasks; task++)
   {
      if (tasks_to_disable[task]) tasks[task] = false;
      if (tasks_to_enable[task])  tasks[task] = true;

      tasks_to_disable[task] = false;
      tasks_to_enable[task]  = false;
   }
}

void Task_manager::print_status( void)
{
   std::cout << "enabled tasks: ";
   for (unsigned int task = 0; task < num_tasks; task++)
   {
      std::cout << tasks[task] << ", ";
   }
   std::cout << std::endl;
}
