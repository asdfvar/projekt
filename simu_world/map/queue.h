#ifndef QUEUE_H
#define QUEUE_H

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

      int abs_pos( int );

      void pop( void );
      void write_chunk( void );

   private:
      unsigned int count;
      int num_chunk_elements;
      QNode* first;
      QNode* last;
};

#endif
