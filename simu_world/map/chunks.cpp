#include "chunks.h"
#include "chunk.h"

Chunks::Chunks( int* size_in )
{
   size[0] = size_in[0];
   size[1] = size_in[1];
   size[2] = size_in[2];

   int total_size = size[0] * size[1] * size[2];

   all_nodes = new Node*[total_size];

   Node* node = NULL;
   Node* row  = NULL;
   Node* vert = NULL;

   base_node = new Node;
   vert      = base_node;
   for (int k = 0; k < size[2]; k++)
   {

      /*   |
      ** --+--+--+--+--
      **   |         
      ** --+--+--+--+--
      **   |         
      ** --+--+--+--+--
      **   |         
      ** --+--+--+--+--
      **   |         
      */
      row = vert;
      for (int j = 0; j < size[1]; j++)
      {
         node = row;
         for (int i = 0; i + 1 < size[0]; i++)
         {
            node->front       = new Node;
            node->front->back = node;
            node->next        = node->front;
            node              = node->front;
         }
         node->front = row;
         row->back   = node;

         if (j + 1 < size[1])
         {
            row->left        = new Node;
            row->left->right = row;
            row              = row->left;
            node->next       = row;
         }
      }
      row->left   = vert;
      vert->right = row;

      /*   |  |  |  |
      ** --+--+--+--+--
      **   |  |  |  |
      ** --+--+--+--+--
      **   |  |  |  |
      ** --+--+--+--+--
      **   |  |  |  |
      ** --+--+--+--+--
      **   |  |  |  |
      */
      row = vert;
      Node* row1 = row;
      Node* row2 = row->left;
      for (int j = 0; j < size[1]; j++)
      {
         for (int i = 0; i < size[0]; i++)
         {
            row1->left  = row2;
            row2->right = row1;

            row1 = row1->front;
            row2 = row2->front;
         }
         row  = row->left;
         row1 = row;
         row2 = row->left;
      }

      if (k + 1 < size[2])
      {
         vert->top               = new Node;
         vert->top->bottom       = vert;
         vert->back->right->next = vert->top;
         vert                    = vert->top;
      }

   }

   vert->top              = base_node;
   base_node->bottom      = vert;
   row->back->right->next = vert->top;
   vert                   = vert->top;

   // connect all nodes between each plane
      /*    /  /  /  /
      **   +  +  +  +  
      **    /  /  /  /
      **   +  +  +  +  
      **    /  /  /  /
      **   +  +  +  +  
      **    /  /  /  /
      **   +  +  +  +  
      **             
      */
   Node* vert1 = base_node;
   Node* vert2 = base_node->top;
   for (int k = 0; k < size[2]; k++)
   {
      Node* row1 = vert1;
      Node* row2 = vert2;
      for (int j = 0; j < size[1]; j++)
      {
         for (int i = 0; i < size[0]; i++)
         {
            row1->top    = row2;
            row2->bottom = row1;

            row1 = row1->front;
            row2 = row2->front;
         }
         row1 = row1->left;
         row2 = row2->left;
      }
      vert1 = vert1->top;
      vert2 = vert2->top;
   }

   current_node = base_node;

   for (int k = 0; k < total_size; k++)
   {
      all_nodes[k] = current_node;
      current_node = current_node->next;
   }

   current_node = base_node;
}

/*
** function name: insert_chunk from: Chunks
**
** set the new chunk in this node then advance to
** the next node.
*/
void Chunks::insert_chunk( Chunk* new_chunk )
{
   current_node->chunk = new_chunk;
   current_node        = current_node->next;
}

/*
** function name: set_base from: Chunks
*/
void Chunks::set_base( void )
{
   current_node = base_node;
}

/*
** function name: next from: Chunks
*/
void Chunks::next( void )
{
   current_node = current_node->next;
}

/*
** function name: at from: Chunks
*/
Chunk* Chunks::at( int chunk_ind )
{
   return all_nodes[chunk_ind]->chunk;
}

int Chunks::get_block( int ind_x, int ind_y, int ind_z )
{
   int chunk_dim[3];
   chunk_dim[0] = current_node->chunk->get_dimension(0);
   chunk_dim[1] = current_node->chunk->get_dimension(1);
   chunk_dim[2] = current_node->chunk->get_dimension(2);

   int block = 0;

   Node* block_node = current_node;

   while ( ind_x >= chunk_dim[0] )
   {
      block_node = block_node->front;
      ind_x -= chunk_dim[0];
   }
   while ( ind_y >= chunk_dim[1] )
   {
      block_node = block_node->left;
      ind_y -= chunk_dim[1];
   }
   while ( ind_z >= chunk_dim[2] )
   {
      block_node = block_node->top;
      ind_z -= chunk_dim[2];
   }
   while ( ind_x < 0 )
   {
      block_node = block_node->back;
      ind_x += chunk_dim[0];
   }
   while ( ind_y < 0 )
   {
      block_node = block_node->right;
      ind_y += chunk_dim[1];
   }
   while ( ind_z < 0 )
   {
      block_node = block_node->bottom;
      ind_z += chunk_dim[2];
   }

   int block_ind = ind_z * chunk_dim[0] * chunk_dim[1] +
                   ind_y * chunk_dim[0]                +
                   ind_x;

   block = block_node->chunk->get_position( NULL, block_ind );

   return block;

}

/*
** destructor name: ~Chunks
*/
Chunks::~Chunks( void )
{
   const int total_size = size[0] * size[1] * size[2];

   Node* node      = base_node;
   Node* node_next = node->next;

   for (int k = 0; k < total_size; k++)
   {
      node_next = node->next;
      delete node;
      node      = node_next;
   }

   delete[] all_nodes;
}

/*
** destructor name: ~Node
*/
Node::~Node( void )
{
   delete chunk;
}
