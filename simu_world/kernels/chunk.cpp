#include "map.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "fileio.h"

/*
** constructor name: Chunk 
*/
Chunk::Chunk( unsigned int  id_in,
              int           abs_pos_id_in[3],
              int          *chunk_dim_in,
              float        *position_in)
{

   abs_pos_id[0] = abs_pos_id_in[0];
   abs_pos_id[1] = abs_pos_id_in[1];
   abs_pos_id[2] = abs_pos_id_in[2];

   prev_abs_pos_id[0] = abs_pos_id[0];
   prev_abs_pos_id[1] = abs_pos_id[1];
   prev_abs_pos_id[2] = abs_pos_id[2];

   for (int ind = 0; ind < 3; ind++)
   {
      if (chunk_dim_in[ind] % 2 == 0)
      {
         std::cout << "Chunk dimension must be odd" << std::endl;
      }
   }

   for (int ind = 0; ind < 3; ind++) chunk_dim[ind] = chunk_dim_in[ind];

   blocks = new int[chunk_dim[0] * chunk_dim[1] * chunk_dim[2]];

   id = id_in;

   for (int ind = 0; ind < 3; ind++) position[ind] = position_in[ind];

   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   valid           = false;
   changed         = false;
   first_populated = true;

   /*
   ** Attempt to read from file upon creation
   */
   generate_chunk();

}

/*
** function name: update from: Chunk
**
** write old data to file and read new
** data to memory if it exists. Otherwise, create it.
** chunks are written to file if they leave the field
** of view (a.k.a. chunks are reassigned).
*/
void Chunk::update( void )
{
   if ( (changed && !valid) || first_populated )
   {
      /*
      ** write old data to file
      */
      write_chunk();

      changed         = false;
      first_populated = false;
   }

   if ( !valid )
   {
      /*
      ** Attempt to read from file
      */
      generate_chunk();

      valid = true;
   }
}

/*
** function name: create_random from: Chunk 
*/
void Chunk::create_random( void )
{
   int chunk_dim_tot = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
   for (int ind = 0; ind < chunk_dim_tot; ind++)
   {
      int random_num = std::rand() % 100;
      if (random_num == 0)
         blocks[ind] = 1;
      else
         blocks[ind] = 0;
   }
}

/*
** function name: create_flat from: Chunk 
*/
void Chunk::create_flat( void )
{
std::cout << "abs_pos_id = " << abs_pos_id[0] << ", "
           << abs_pos_id[1] << ", " << abs_pos_id[2] << std::endl;
   int chunk_dim_tot = chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
   if (abs_pos_id[2] < 0)
   {
      for (int ind = 0; ind < chunk_dim_tot; ind++)
      {
         blocks[ind] = 1;
      }
   }
   else
   {
      for (int ind = 0; ind < chunk_dim_tot; ind++)
      {
         blocks[ind] = 0;
      }
   }
}

/*
** function name: display_info from: Chunk
*/
void Chunk::display_info( void)
{
   bool block_exists = false;
   float l_position[3];
   for (int ind = 0; ind < chunk_dim[0] * chunk_dim[1] * chunk_dim[2]; ind++)
   {
      if (blocks[ind] > 0)
      {
         block_exists = true;
         int stat = get_position( l_position, ind);
         if (stat != 0)
         {
            std::cout << "block position = " << l_position[0] << ", "
                                             << l_position[1] << ", "
                                             << l_position[2] << std::endl;
         }
      }
      
   }

   if (block_exists)
   {
      std::cout << "block exists" << std::endl;
      std::cout << "Chunk position = " << position[0] << ", "
                                       << position[1] << ", "
                                       << position[2] << std::endl;
   }
}

/*
** function name: position_in_chunk from Chunk 
**
** returns true if the position is inside the Chunk 
** with the boundaries defined as:
** [ , ) x [ , ) x [ , )
*/
bool Chunk::position_in_chunk( float *position_in)
{
   if (position_in[0] >= position[0]                                    &&
       position_in[0] <  position[0] + static_cast<float>(chunk_dim[0]) &&
       position_in[1] >= position[1]                                    &&
       position_in[1] <  position[1] + static_cast<float>(chunk_dim[1]) &&
       position_in[2] >= position[2]                                    &&
       position_in[2] <  position[2] + static_cast<float>(chunk_dim[2]))
   {
      return true;
   }

   return false;
}

/*
** function name: move from Chunk 
**
** move the chunk a discrete number of chunks
*/
void Chunk::move( int x, int y, int z)
{

   prev_abs_pos_id[0] = abs_pos_id[0];
   prev_abs_pos_id[1] = abs_pos_id[1];
   prev_abs_pos_id[2] = abs_pos_id[2];

   abs_pos_id[0] += x;
   abs_pos_id[1] += y;
   abs_pos_id[2] += z;

   position[0] += static_cast<float>(x) * chunk_dim[0];
   position[1] += static_cast<float>(y) * chunk_dim[1];
   position[2] += static_cast<float>(z) * chunk_dim[2];

   std::cout << "moving chunk " << prev_abs_pos_id[0] << ", "
                                << prev_abs_pos_id[1] << ", "
                                << prev_abs_pos_id[2]
             << " to "
                                << abs_pos_id[0] << ", "
                                << abs_pos_id[1] << ", "
                                << abs_pos_id[2]
                                << std::endl;

   valid   = false;
   //changed = true;
}

/*
** destructor name: ~Chunk
*/
Chunk::~Chunk(void)
{
   delete[] blocks;
}

/*
** function name: get_dimensions from Chunk 
*/
unsigned int Chunk::get_dimensions(void)
{
   return chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
}

/*
** function name: get_dimensions from Chunk 
*/
unsigned int Chunk::get_dimensions( unsigned int *dimensions_out)
{
   dimensions_out[0] = chunk_dim[0];
   dimensions_out[1] = chunk_dim[1];
   dimensions_out[2] = chunk_dim[2];

   return chunk_dim[0] * chunk_dim[1] * chunk_dim[2];
}

/*
** function name: get_position from Chunk
*/
int Chunk::get_position(float* position_out,
                        int    block_index)
{

   if ( position_out == NULL ) return blocks[block_index];

   if (block_index >= chunk_dim[0] * chunk_dim[1] * chunk_dim[2])
   {
      std::cout << "block index exceeds chunk dimensions" << std::endl;
      return 0;
   }

   // get block x, y, z indices
   unsigned int block_index_x = block_index  %  chunk_dim[0];
   unsigned int block_index_y = (block_index % (chunk_dim[0] * chunk_dim[1])) / chunk_dim[0];
   unsigned int block_index_z = block_index  / (chunk_dim[0] * chunk_dim[1]);

   // get relative position to this block
   float rel_block_pos_x = static_cast<float>(block_index_x);
   float rel_block_pos_y = static_cast<float>(block_index_y);
   float rel_block_pos_z = static_cast<float>(block_index_z);

   // convert to absolute position
   float block_pos_x = rel_block_pos_x + position[0];
   float block_pos_y = rel_block_pos_y + position[1];
   float block_pos_z = rel_block_pos_z + position[2];

   position_out[0] = block_pos_x + 0.5f;
   position_out[1] = block_pos_y + 0.5f;
   position_out[2] = block_pos_z + 0.5f;

   return blocks[block_index];

}

/*
** function name: get_block from Chunk
*/
int Chunk::get_block( int *block_index_in)
{
   if (block_index_in[0] * block_index_in[1] * block_index_in[2] >=
       chunk_dim[0] * chunk_dim[1] * chunk_dim[2])
   {
      std::cout << "block index exceeds chunk dimensions" << std::endl;
      return 0;
   }

   return blocks[ block_index_in[2] * chunk_dim[2] * chunk_dim[1] +
                  block_index_in[1] * chunk_dim[0]                +
                  block_index_in[0] ];
}

/*
** function name: get_abs_pos_id from Chunk
*/
void Chunk::get_abs_pos_id( int *abs_pos_id_out )
{
   abs_pos_id_out[0] = abs_pos_id[0];
   abs_pos_id_out[1] = abs_pos_id[1];
   abs_pos_id_out[2] = abs_pos_id[2];
}

/*
** function name: is_valid from Chunk
*/
bool Chunk::is_valid( void )
{
   return valid;
}

/*
** function name: generate_chunk from Chunk
*/
void Chunk::generate_chunk( void )
{
#ifdef __linux__
   std::string chunk_name = "saves/chunk_";
#elif _WIN32
   std::string chunk_name = "saves\chunk_";
#else
   std::cout << __FILE__ << ":" << __LINE__ << ": unknown OS" << std::endl;
#endif

   std::ostringstream id_str;
   if (abs_pos_id[0] < 0)
   {
      id_str << -abs_pos_id[0];
      chunk_name += "n";
   }
   else
   {
      id_str << abs_pos_id[0];
   }
   chunk_name += id_str.str();
   chunk_name += "_";

   id_str.clear();
   id_str.str("");
   if (abs_pos_id[1] < 0)
   {
      id_str << -abs_pos_id[1];
      chunk_name += "n";
   }
   else
   {
      id_str << abs_pos_id[1];
   }
   chunk_name += id_str.str();
   chunk_name += "_";

   id_str.clear();
   id_str.str("");
   if (abs_pos_id[2] < 0)
   {
      id_str << -abs_pos_id[2];
      chunk_name += "n";
   }
   else
   {
      id_str << abs_pos_id[2];
   }
   chunk_name += id_str.str();

   if( !fio::read( chunk_name,       // path and file name included
                   0,                // offset in bytes from the beginning of file
                   (void*)blocks,    // buffer to hold the data
                   chunk_dim[0] *
                      chunk_dim[1] *
                      chunk_dim[2] *
                      sizeof(chunk_dim[0])) ) // number of bytes to read
   {
      std::cout << "new chunk (" << abs_pos_id[0] << ", "
                                 << abs_pos_id[1] << ", "
                                 << abs_pos_id[2] << ")"
                                 << std::endl;
      create_random();
//      create_flat();
      changed         = true;
   }
   else
   {
      std::cout << "reading chunk (" << abs_pos_id[0] << ", "
                                     << abs_pos_id[1] << ", "
                                     << abs_pos_id[2] << ")"
                                     << std::endl;
      changed         = false;
      first_populated = false;
   }

   valid = true;
}

/*
** function name: write_chunk from Chunk
*/
void Chunk::write_chunk( void )
{

      std::string chunk_name = "saves/chunk_";
      std::ostringstream id_str;

      if (prev_abs_pos_id[0] < 0)
      {
         id_str << -prev_abs_pos_id[0];
         chunk_name += "n";
      }
      else
      {
         id_str << prev_abs_pos_id[0];
      }
      chunk_name += id_str.str();
      chunk_name += "_";

      id_str.clear();
      id_str.str("");
      if (prev_abs_pos_id[1] < 0)
      {
         id_str << -prev_abs_pos_id[1];
         chunk_name += "n";
      }
      else
      {
         id_str << prev_abs_pos_id[1];
      }
      chunk_name += id_str.str();
      chunk_name += "_";

      id_str.clear();
      id_str.str("");
      if (prev_abs_pos_id[2] < 0)
      {
         id_str << -prev_abs_pos_id[2];
         chunk_name += "n";
      }
      else
      {
         id_str << prev_abs_pos_id[2];
      }
      chunk_name += id_str.str();

      std::cout << "writing chunk (" << prev_abs_pos_id[0] << ", "
                                     << prev_abs_pos_id[1] << ", "
                                     << prev_abs_pos_id[2] << ") "
                                     << chunk_name << std::endl;

      fio::write( chunk_name,
                  0,
                 (char*)blocks,
                  chunk_dim[0] *
                  chunk_dim[1] *
                  chunk_dim[2] * sizeof(chunk_dim[0]));
}

void Chunk::set_color( float *color_in )
{
   color[0] = color_in[0];
   color[1] = color_in[1];
   color[2] = color_in[2];
}

void Chunk::get_color( float *color_out )
{
   color_out[0] = color[0];
   color_out[1] = color[1];
   color_out[2] = color[2];
}

Chunks_new::Chunks_new( int* size_in )
{
   size[0] = size_in[0];
   size[1] = size_in[1];
   size[2] = size_in[2];

   int total_size = size[0] * size[1] * size[2];

   all_nodes = new Node*[total_size];

   Node* node;
   Node* row;
   Node* vert;

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

#if 0
{
printf("base_node = %ld\n", (long int)base_node);
Node* temp = base_node->right;

for (int kk = 0; kk < size[2]; kk++) {
   std::cout << "plane " << kk << std::endl;
   for (int jj = 0; jj < size[1]; jj++) {
      for (int ii = 0; ii < size[0]; ii++) {
         printf("%ld,%ld,%ld,", (long int)temp->top, (long int)temp->left, (long int)temp->next);
         temp = temp->front;
      }
      std::cout << std::endl;
      for (int ii = 0; ii < size[0]; ii++) {
         printf("%ld,%ld,%ld,", (long int)temp->back, (long int)temp, (long int)temp->front);
         temp = temp->front;
      }
      std::cout << std::endl;
      for (int ii = 0; ii < size[0]; ii++) {
         printf("%ld,%ld,,", (long int)temp->bottom, (long int)temp->right);
         temp = temp->front;
      }
      temp = temp->right;
      std::cout << std::endl;
   }
   std::cout << std::endl;
   temp = temp->top;
}
std::cout << "next list" << std::endl;
temp = base_node;
for (int kk = 0; kk < size[0]*size[1]*size[2] + 1; kk++) {
   printf("%ld\n", (long int)temp);
   temp = temp->next;
}
}
#endif

   for (int k = 0; k < total_size; k++)
   {
      all_nodes[k] = current_node;
      current_node = current_node->next;
   }

   current_node = base_node;

}

/*
** function name: insert_chunk from: Chunks_new
**
** set the new chunk in this node then advance to
** the next node.
*/
void Chunks_new::insert_chunk( Chunk* new_chunk )
{
   current_node->chunk = new_chunk;
   current_node = current_node->next;
}

/*
** function name: set_base from: Chunks_new
*/
void Chunks_new::set_base( void )
{
   current_node = base_node;
}

/*
** function name: at from: Chunks_new
*/
Chunk* Chunks_new::at( int chunk_ind )
{
   return all_nodes[chunk_ind]->chunk;
}

/*
** destructor name: ~Chunks_new
*/
Chunks_new::~Chunks_new( void )
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
