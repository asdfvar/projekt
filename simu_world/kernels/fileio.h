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

 bool read (const std::string path);

 bool read (const std::string  path,
            int                position,
            void              *buffer,
            int                N);

 void directory ( void );

}

#endif
