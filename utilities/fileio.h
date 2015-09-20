#ifndef FILEIO_H
#define FILEIO_H

#include <string>

namespace fio {

void write_bin_float (const std::string  path,
                      float             *data,
                      bool               append,
                      int                N);

} // namespace fio

#endif
