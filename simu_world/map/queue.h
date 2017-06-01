#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <stdlib.h>

class QNode
{
   public:

      void write_chunk( int num_chunk_elements );

      int* chunk_elements;
      QNode* next;
     ~QNode( void );
      int abs_pos_x;
      int abs_pos_y;
      int abs_pos_z;

//
      void write( void );

#ifdef BLOCKS
      QNode( const std::string& file,
             int*  data,
             int   data_id,
             int   data_size );

   private:

      std::string file;
      int*        data;
      int         data_size;
      int         data_id;
#endif

};

/*
** class name: Queue
*/
class Queue
{
   public:
      Queue( int num_chunk_elements );
     ~Queue( void );

      void new_chunk( int* chunk_elements,
                      int  abs_pos_x,
                      int  abs_pos_y,
                      int  abs_pos_z );

      void write_chunk( void );

      void fill_buffer( const std::string& file,
                        int*  data,
                        int   data_id,
                        int   data_size );

      void write_all( void );

   private:

      unsigned int count;
      int num_chunk_elements;
      QNode* first;
      QNode* last;

      QNode* first_write;
      QNode* last_write;
      QNode* first_read;
      QNode* last_read;
};

std::string create_filename( int   chunk_x,
                             int   chunk_y,
                             int   chunk_z );

#endif
