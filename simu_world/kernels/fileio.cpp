#include "fileio.h"
#include <string>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <stdlib.h>

namespace fio
{

  void directory( void )
  {

#if 0
     DIR *dir = opendir("saves");

     if (dir)
     {
        std::cout << "Directory exists" << std::endl;
        return;
     }
     else if (ENOENT == errno)
     {
        std::cout << "Directory DOESN'T exists" << std::endl;
        // create directory
//        mkdir("saves");
     }
#endif

     std::cout << "system output = " << system("mkdir saves") << std::endl;

  }

 /*
 ** function NAME: write_bin_float
 **
 ** Write float data to file
 */
 void write (const std::string  path,
             int                position,
             char              *data,
             int                N)
 {

    std::ofstream OF;

    // open the file for reading and writting
    OF.open( path.c_str(), std::ios::out | std::ios::in | std::ios::binary );

    // if the file doesn't exist, open for writting only
    if( !OF.is_open() )
    {
       OF.open( path.c_str(), std::ios::out | std::ios::binary );
    }

    OF.seekp( position, std::ios::beg);
    OF.write( (char*)data, N * sizeof(*data) );
    OF.close();

 }

 void read (const std::string  path,     // path and file name included
            int                position, // offset in bytes from the beginning of file
            void              *buffer,   // buffer to hold the data
            int                N)        // number of bytes to read
 {
    std::ios_base::seekdir position_x;
    std::ifstream IS ( path.c_str(), std::ifstream::binary );
    IS.seekg( position );
    IS.read ((char*)buffer, N);
    IS.close();
 }

 void read (const std::string path)
 {
    std::ifstream IS ( path.c_str(), std::ifstream::binary );

    // get length of file:
    IS.seekg(0, IS.end);
    int length = IS.tellg();
    IS.seekg (0, IS.beg);

    int *buffer = new int[length];
    IS.read ((char*)buffer, length);
    IS.close();

    std::cout << "reading " << length << " bytes" << std::endl;
    for (int k = 0; k < length/4; k++)
       std::cout << buffer[k];
    std::cout << std::endl;

    delete[] buffer;

 }

}
