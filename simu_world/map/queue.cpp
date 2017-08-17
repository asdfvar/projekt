#include "fileio.h"
#include "queue.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

std::string create_filename( int   chunk_x,
                             int   chunk_y,
                             int   chunk_z )
{

   std::string filename;
#ifdef __linux__
   filename = "saves/chunk_";
#elif _WIN32
   filename = "saves\chunk_";
#else
   std::cout << __FILE__ << ":" << __LINE__ << ": unknown OS" << std::endl;
#endif

   std::ostringstream id_str;
   if (chunk_x < 0)
   {
      id_str << -chunk_x;
      filename += "n";
   }
   else
   {
      id_str << chunk_x;
   }
   filename += id_str.str();
   filename += "_";

   id_str.clear();
   id_str.str("");
   if (chunk_y < 0)
   {
      id_str << -chunk_y;
      filename += "n";
   }
   else
   {
      id_str << chunk_y;
   }
   filename += id_str.str();
   filename += "_";

   id_str.clear();
   id_str.str("");
   if (chunk_z < 0)
   {
      id_str << -chunk_z;
      filename += "n";
   }
   else
   {
      id_str << chunk_z;
   }
   filename += id_str.str();

   return filename;
}

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
** function name: write_chunk from: Queue
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
   delete[] data;
}

/*
** constructor name: Queue
*/
Queue::Queue( int num_chunk_elements_in )
{
   num_chunk_elements = num_chunk_elements_in;
   first = last = 0;
   count = 0;
   read_count = 0;
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


QNode::QNode( const std::string& file_in,
              int*               data_in,
              int                data_id_in,
              int                data_size_in )
{
   file      = file_in;
   data_id   = data_id_in;
   data_size = data_size_in;

   data = new int[data_size];

   for (int k = 0; k < data_size; k++) data[k] = data_in[k];
}

void Queue::fill_buffer( const std::string& file,
                         int*               data,
                         int                data_id,
                         int                data_size )
{

   std::cout << "added to queue: " << file << std::endl;

   if( count == 0 )
   {
      first = last = new QNode( file, data, data_id, data_size );
   }
   else
   {
      last->next  = new QNode( file, data, data_id, data_size );
      last        = last->next;
   }

   count++;

}

void Queue::write_one( void )
{
   if (count > 0)
   {
      std::cout << "writing chunk " << count << std::endl;
      first->write();

      QNode* qnode = first;
      if (first != last) first = first->next;
      delete qnode;

      count--;
   }
}

void Queue::write_all( void )
{
   while ( count > 0 )
   {
      std::cout << "writing chunk " << count << std::endl;
      first->write();

      QNode* qnode = first;
      if (first != last) first = first->next;
      delete qnode;

      count--;
   }
}

void QNode::write( void )
{
   std::cout << __FILE__ << ":" << __LINE__ <<
               ":writing data to " << file << std::endl;;

   fio::write( file,
               0,
              (char*)data,
               data_size );
}

void Queue::add_read( const std::string& file, int data_size )
{

   std::cout << "added to queue: " << file << std::endl;

   int* data = new int[data_size];

   if( read_count == 0 )
   {
      first_read = last_read = new QNode( file, data, 0, data_size );
   }
   else
   {
      last_read->next  = new QNode( file, data, 0, data_size );
      last_read        = last_read->next;
   }

   read_count++;

}
