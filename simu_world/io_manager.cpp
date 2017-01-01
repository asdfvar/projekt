#include "io_manager.h"
#include <iostream>

void *io_manager( void *args_in )
{
   while( true )
      std::cout << "got here" << std::endl;
}
