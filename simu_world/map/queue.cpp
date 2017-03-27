#include "fileio.h"
#include "queue.h"
#include <iostream>
#include <sstream>

/*
** function name: write_chunk from QNode
*/
void QNode::write_chunk( int num_chunk_elements )
{
   std::string chunk_name = "saves/chunk_";
   std::ostringstream id_str;
 
       if (abs_pos_x < 0)
       {
          id_str << -abs_pos_x;
          chunk_name += "n";
       }
       else
       {
          id_str << abs_pos_x;
       }
       chunk_name += id_str.str();
       chunk_name += "_";
 
       id_str.clear();
       id_str.str("");
       if (abs_pos_y < 0)
       {
          id_str << -abs_pos_y;
          chunk_name += "n";
       }
       else
       {
          id_str << abs_pos_y;
       }
       chunk_name += id_str.str();
       chunk_name += "_";
 
       id_str.clear();
       id_str.str("");
       if (abs_pos_z < 0)
       {
          id_str << -abs_pos_z;
          chunk_name += "n";
       }
       else
       {
          id_str << abs_pos_z;
       }
       chunk_name += id_str.str();
 
       std::cout << __FILE__ << ":" << __LINE__ <<
                   " writing chunk (" << abs_pos_x << ", "
                                      << abs_pos_y << ", "
                                      << abs_pos_z << ")"
                                      << std::endl;
 
       fio::write( chunk_name,
                   0,
                  (char*)chunk_elements,
                   num_chunk_elements * sizeof(*chunk_elements));
}

/*
** function name: write_chunk from Queue
*/
void Queue::write_chunk( void )
{
   while ( count > 0 )
   {
      std::cout << "writing chunk " << count << std::endl;
      first->write_chunk( num_chunk_elements );

      QNode* qnode = first;
      if (first != last) first = first->next;
      delete qnode;

      count--;
   }
}

/*
** destructor name: QNode
*/
QNode::~QNode( void )
{
   std::cout << "deleting chunk_elements at " <<
              abs_pos_x << ", " <<
              abs_pos_y << ", " <<
              abs_pos_z << std::endl
              << std::endl;

   delete chunk_elements;
}

/*
** constructor name: Queue
*/
Queue::Queue( int num_chunk_elements_in )
{
   num_chunk_elements = num_chunk_elements_in;
   first = last = 0;
   count = 0;
}

/*
** destructor name: Queue
*/
Queue::~Queue( void )
{
   while( count > 0 )
   {
      QNode* qnode = first;
      if( count > 1 ) first = first->next;
      delete qnode;

      count--;
   }

}

/*
** function: new_chunk from: Queue
*/
void Queue::new_chunk( int* chunk_elements,
                       int  abs_pos_x,
                       int  abs_pos_y,
                       int  abs_pos_z )
{

   std::cout << __FILE__ << ":" << __LINE__ << ":new chunk at ("
             << abs_pos_x << ", "
             << abs_pos_y << ", "
             << abs_pos_z << ")"
             << std::endl;

   if( count == 0 )
   {
      first = last = new QNode;
   }
   else
   {
      last->next  = new QNode;
      last        = last->next;
   }

   last->chunk_elements = chunk_elements;
   last->abs_pos_x = abs_pos_x;
   last->abs_pos_y = abs_pos_y;
   last->abs_pos_z = abs_pos_z;

   count++;
}
