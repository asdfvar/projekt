#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>

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

      void list_contents   (void);
      void test_ends       (void);
      bool test_duplicates (void);
      bool test_nulls      (void);

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

      ~Lattice (void);

      Type *access (int *index);

      Type *next (void)
      {
         if (current_node != last_node)
         {
            current_node = current_node->flat_next;

            current_index[0]++;
            for (int ind = 0; ind < dim; ind++)
            {
               if (current_index[ind] >= size[ind])
               {
                  current_index[ind] = 0;
                  current_index[ind+1]++;
               }
            }
         }
         return current_node->object;
      };

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

      int  *current_index;
      Node *current_node;

      Node *first_node;
      Node *last_node;
};

#endif
