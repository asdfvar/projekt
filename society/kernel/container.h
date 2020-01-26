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
      Type *pop       (void                   );
      Type *pop_back  (void                   );

      // Set the container size to zero. Does not remove the contents
      void reset      (void                   );

      // Access the specified object
      Type *access    (int index              );

      // Access the current object
      Type *access (void);

      // Advance the current object pointer to the next cyclically
      void advance (void);

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

#endif
