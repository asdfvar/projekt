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

      void insert (Type *object, int index);
      Type *access (int index);
      Type *pop (int index);

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

      bool advance_to_index (int index);
};

#endif