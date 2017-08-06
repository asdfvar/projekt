#ifndef READ_CHUNK_H
#define READ_CHUNK_H

#include <string>

void create_random( int* blocks, int  num_chunk_elements );

void generate_chunk( int* blocks, int num_chunk_elements );

bool read_chunk( std::string& filename,
                 int*         chunk_elements,
                 int          num_chunk_elements );

#endif
