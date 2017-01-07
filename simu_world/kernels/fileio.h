#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include <iostream>

namespace fio
{

 void write (const std::string  path,
             int                position,
             char              *data,
             int                N);

 void read (const std::string path);

#if 1
 void read (const std::string  path,
            int                position,
            void              *buffer,
            int                N);
#endif

}

#endif
