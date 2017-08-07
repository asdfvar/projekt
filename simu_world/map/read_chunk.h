#ifndef READ_CHUNK_H
#define READ_CHUNK_H

#include <string>

void create_random( int* blocks, int  num_chunk_elements );

void generate_chunk( int* blocks,
                     int  pos_x,
                     int  pos_y,
                     int  pos_z,
                     int  dim_x,
                     int  dim_y,
                     int  dim_z );

void generate_chunk2( int* dst,
                      int  pos_x,
                      int  pos_y,
                      int  pos_z,
                      int  dim_x,
                      int  dim_y,
                      int  dim_z );

bool read_chunk( std::string& filename,
                 int*         chunk_elements,
                 int          num_chunk_elements );

#endif
