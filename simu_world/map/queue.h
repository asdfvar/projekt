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

      void write( void );

      QNode( const std::string& file,
             int*  data,
             int   data_id,
             int   data_size );

   private:

      std::string file;
      int*        data;
      int         data_size;
      int         data_id;

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

      void add_read( const std::string& file, int data_size );

   private:

      unsigned int count;
      unsigned int read_count;
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

template<class Type>
class QNode_vector
{
   public:
      QNode_vector( Type* src, int num_elements )
      {
         if (num_elements > 10)
         {
            std::cout << __FILE__ << ":" << __LINE__ <<
               ":too many elements" << std::endl;
            return;
         }

         for (int ind = 0; ind < num_elements; ind++)
         {
            data[ind] = src[ind];
         }
      }

      QNode_vector* next;

   private:
      Type data[10];
      int  N;
};

template<class Type>
class ReadQueue
{
   public:
      ReadQueue( void )
      {
         count = 0;
      }
      ~ReadQueue( void ) { }

         void new_node( Type* src, int N )
         {
            if (count == 0)
            {
               first = new QNode_vector<Type>( src, N );
               last = first;
               first->next = last;
            }
            else
            {
               QNode_vector<Type>* node = last;
               last = new QNode_vector<Type>( src, N );
               node->next = last;
            }
            count++;
         }

      QNode_vector<Type>* pop( void )
      {
         QNode_vector<Type>* node = NULL;

         if (count > 0)
         {
            node = first;
            first = first->next;
            count--;
         }
         return node;
      }

   private:
      unsigned int count;
      QNode_vector<Type>* first;
      QNode_vector<Type>* last;
};

#endif
