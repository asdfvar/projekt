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

#if 0
    for (int k = 0; k < N; k++)
    {
       OF << data[k];
    }
#endif

    OF.write( (char*)data, sizeof(float)*N );

    OF.close();

 }

} // namespace fil
