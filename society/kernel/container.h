#ifndef CONTAINER_H
#define CONTAINER_H

template <typename Type>
class Container
{
   public:

      Container (void)
      {
         container_size = 0;
         current_index  = 0;
      }

      void insert     (Type *object, int index);
      void push_front (Type *object           );
      void push_back  (Type *object           );
      Type *back      (void                   );

      Type *pop       (int index              );
      Type *pop_back  (void                   );

      // Set the container size to zero. Does not remove the contents
      void reset      (void                   );

      // Access the specified object
      Type *access    (int index              );

      int size (void) { return container_size; };

      void list_contents (void);
      void test_ends     (void);

   private:

      struct Node
      {
         Type *object;
         Node *next;
         Node *previous;
      };

      int   container_size;
      int   current_index;
      Node *current_node;
      Node *front_node;
      Node *back_node;

      bool advance (int index);
};

template <typename Type>
class Lattice
{
   public:

      Lattice (int *size_in, int dim_in)
      {
         dim = dim_in;

         size          = new int[dim];
         current_index = new int[dim];

         for (int ind = 0; ind < dim; ind++)
         {
            size[ind] = size_in[ind];
            current_index[ind] = 0;
         }

         // Build the lattice
         int total_size = 1;
         for (int ind = 0; ind < dim; ind++) total_size *= size[ind];

         Node *node = new Node;

         first_node = node;
         first_node->flat_previous = first_node;

         node->object = new Type;

         // String together all the nodes flattened over one dimension
         for (int ind = 1; ind < total_size; ind++)
         {
            node->flat_next = new Node;
            Node *l_node   = node;
            node = node->flat_next;
            node->flat_previous = l_node;
            node->object        = new Type;
         }

         last_node            = node;
         last_node->flat_next = last_node;

         // Build the connections of the lattice to connect at adjacent cells
         int dist = 1;
         for (int rank = 0; rank < dim; rank++)
         {
            Node *node = first_node;
            if (rank > 0) dist *= size[rank - 1];
            while (node != last_node)
            {
               Node *inode = node;

               for (int ind = 0; ind < dist; ind++) inode = inode->flat_next;

               node->next[rank] = inode;

               inode = node;

               for (int ind = 0; ind < dist; ind++) inode = inode->flat_previous;

               node->previous[rank] = inode;
            }
         }

         // Set the current node
         current_node = first_node;
      }

      ~Lattice (void)
      {
         delete[] size;
         delete[] current_index;

         int total_size = 1;
         for (int ind = 0; ind < dim; ind++) total_size *= size[ind];

         Node *node = first_node;

         while (node != last_node)
         {
            delete node->object;
            node = node->flat_next;
            delete node->flat_previous;
         }

         delete node->object;
         delete node;
      }

   private:

      struct Node
      {
         Type *object;
         Node *flat_next;
         Node *flat_previous;
         Node **next;
         Node **previous;
      };

      int dim;
      int size[3];
      int current_index[3];

      Node *current_node;
      Node *first_node;
      Node *last_node;

      Node *access (Node *node, int index)
      {
         node = first_node;
         for (int ind = 0; ind < index; ind++) node = node->flat_next;
      }
};

#endif
