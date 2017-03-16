#ifndef CHUNKS_H
#define CHUNKS_H

#include "chunk.h"

/*
** class name: Node
*/
class Node
{
   public:
     ~Node( void );

      Chunk* chunk;

      Node* front;
      Node* back;
      Node* left;
      Node* right;
      Node* top;
      Node* bottom;
      Node* next;
};

/*
** class name: Chunks
*/
class Chunks
{
   public:

      Chunks( int* size );
     ~Chunks( void );

      void insert_chunk( Chunk* );
      void set_base( void );
      void next( void );
      Chunk* at( int ind );
      int get_block( int ind_x, int ind_y, int ind_z ); 

   private:

      int size[3];
      int position;

      Node* base_node;
      Node* current_node;

      Node** all_nodes;

};

#endif
