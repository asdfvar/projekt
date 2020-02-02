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
         current_node = node;
         current_node->previous = current_node;

         first_node = current_node;

         current_node->object = new Type;

         for (int ind = 1; ind < total_size; ind++)
         {
            current_node->next = new Node;
            Node *node = current_node;
            current_node = current_node->next;
            current_node->previous = node;
            current_node->object = new Type;
         }

         last_node = current_node;
         last_node->next = last_node;

         current_node = first_node;
      }

      ~Lattice (void)
      {
         delete[] size;
         delete[] current_index;

         int total_size = 1;
         for (int ind = 0; ind < dim; ind++) total_size *= size[ind];

         current_node = first_node;

         while (current_node != last_node)
         {
            delete current_node->object;
            current_node = current_node->next;
            delete current_node->previous;
         }

         delete current_node->object;
         delete current_node;
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
};

#endif
