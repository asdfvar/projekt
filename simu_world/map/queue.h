#ifndef QUEUE_H
#define QUEUE_H

#include "chunks.h"

/*
** class name: Queue
*/
class Queue
{
   public:
      Queue( void );
     ~Queue( void );

      void new_chunk( Chunk* );
      void pop( void );

   private:
      Node* first;
      Node* last;
};

#endif
