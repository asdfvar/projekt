#include "semaphore.h"
#include <iostream>

Semaphore::Semaphore( unsigned int num_tasks_in)
{
   num_tasks   = num_tasks_in;

   tasks             = new int[num_tasks];

   for (unsigned int task = 0; task < num_tasks; task++)
   {
      tasks[task]            = 0;
   }
}

Semaphore::~Semaphore( void)
{
   delete[] tasks;
}

void Semaphore::print_status( void)
{
   std::cout << "enabled tasks: ";
   for (unsigned int task = 0; task < num_tasks; task++)
   {
      std::cout << tasks[task] << ", ";
   }
   std::cout << std::endl;
}

int Semaphore::increment_task( int task)
{

   if (task > num_tasks)
   {
      std::cout << "task ID exceeds the number of tasks available" << std::endl;
      return -1;
   }
   tasks[task]++;

   return 0;
}

int  Semaphore::decrement_task( int task)
{

   if (task > num_tasks)
   {
      std::cout << "task ID exceeds the number of tasks available" << std::endl;
      return -1;
   }
   else if (tasks[task] == 0)
   {
      std::cout << "task " << task << " already zero" << std::endl;
      return -1;
   } else {
      tasks[task]--;
   }

   return 0;
}

int Semaphore::task_pool( int task)
{
   return tasks[task];
}

void Semaphore::wait_for_task( int task)
{
   while( tasks[task] > 0 ) { /* waiting */ }
}
