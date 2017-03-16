#include "queue.h"

/*
** constructor name: Queue
*/
Queue::Queue( void )
{
   first = last = 0;
}

/*
** destructor name: Queue
*/
Queue::~Queue( void )
{
   while( first != last )
   {
      Node* node = first->next;
      delete first;
      first = node;
   }

   if ( first != 0 ) delete first;
}

/*
** function: new_chunk from: Queue
*/
void Queue::new_chunk( Chunk* chunk )
{
   if( last == 0)
   {
      last = new Node;
      first = last;
   }
   else
   {
      last->next  = new Node;
      last        = last->next;
   }
   last->chunk = chunk;

   if( first == 0)
   {
      first = last;
   }

}

/*
** function: pop from: Queue
*/
void Queue::pop( void )
{
   Node* node = first;
   if (first != last) first = first->next;

   /*
   ** delete will write to file (eventually)
   */
   delete node;
}
