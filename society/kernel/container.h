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

      Lattice (int *size_in, int dim_in);

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
            delete node->next;
            delete node->previous;
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
      int *size;
      int *current_index;

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
