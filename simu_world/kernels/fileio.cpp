#include "fileio.h"
#include <string>
#include <fstream>
#include <iostream>

namespace fio
{

 /*
 ** Function NAME: write_bin_float
 **
 ** Write float data to file
 */
 void write (const std::string  path,
             int                position,
             char              *data,
             int                N)
 {

    std::ofstream OF;

    OF.open( path.c_str(), std::ios::out | std::ios::in | std::ios::binary );
    OF.seekp( position, std::ios::beg);
    OF.write( (char*)data, N * sizeof(*data) );
    OF.close();

   std::cout << "size of char = " << sizeof(char) << " size of int = " << sizeof(int) << std::endl;
 }

 void read (const std::string  path,
            int                position,
            void              *buffer,
            int                N)
 {
    std::ios_base::seekdir position_x;
    std::ifstream IS ( path.c_str(), std::ifstream::binary );
    IS.seekg( position );
    IS.read ((char*)buffer, N);
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

    std::cout << "reading " << length << " bytes" << std::endl;
    for (int k = 0; k < length/4; k++)
       std::cout << buffer[k];
    std::cout << std::endl;

    delete[] buffer;

 }

}
