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
      Type *access    (int index              );
      Type *pop       (int index              );
      Type *pop_back  (void                   );
      void reset      (void                   );

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

      bool advance_to_index (int index);
};

#endif
