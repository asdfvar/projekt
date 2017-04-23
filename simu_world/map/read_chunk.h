#ifndef READ_CHUNK_H
#define READ_CHUNK_H

void create_random( int* blocks, int  num_chunk_elements );

void read_chunk( int* chunk_elements,
                 int ind_x,
                 int ind_y,
                 int ind_z,
                 int num_chunk_elements );

#endif
