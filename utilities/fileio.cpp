#include <string>
#include <fstream>
#include <iostream>

namespace fio {

 /*
 ** Function NAME: write_bin_float
 **
 ** Write float data to file
 */
 void write_bin_float (const std::string  path,
                       float             *data,
                       bool               append,
                       int                N)
 {

    std::ofstream OF;

    if (append) OF.open( path.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    else OF.open( path.c_str(), std::ios::out | std::ios::binary );

    OF.write( (char*)data, sizeof(float)*N );

    OF.close();

 }

 /*
 ** Function NAME: write_bin_int
 **
 ** Write float data to file
 */
 void write_bin_int (const std::string  path,
                     int               *data,
                     bool               append,
                     int                N)
 {

    std::ofstream OF;

    if (append) OF.open( path.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    else OF.open( path.c_str(), std::ios::out | std::ios::binary );

    OF.write( (char*)data, sizeof(float)*N );

    OF.close();

 }

} // namespace fil
