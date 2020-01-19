#include "container.h"
#include "actions.h"
#include <iostream>

template <typename Type>
void Container<Type>::push_front (Type *object)
{
   insert (object, 0);
}

template <typename Type>
void Container<Type>::insert (Type *object, int index)
{
   Node *node = new Node;
   node->object = object;

   if (container_size == 0)
   {
      front_node   = back_node = node;
      node->next   = node->previous = node;
      current_node = node;
   }
   else
   {
      // advance to the desired object node index
      bool stat = advance_to_index (index);
      if (!stat) return;
   }

   if (current_node == front_node)
      front_node = node;

   // Attach the new node
   node->previous = current_node->previous;
   node->next     = current_node;

   // Configure the linkage for neighboring nodes
   current_node->previous = node;
   node->previous->next   = node;

   // The current node becomes the new node
   current_node = node;

   container_size++;
}

template <typename Type>
Type *Container<Type>::access (int index)
{
   // advance to the desired object node index
   bool stat = advance_to_index (index);
   if (!stat) return nullptr;

   return current_node->object;
}

template <typename Type>
Type *Container<Type>::pop (int index)
{
   if (container_size == 0) return nullptr;

   // advance to the desired object node index
   bool stat = advance_to_index (index);
   if (!stat) return nullptr;

   Type *object = current_node->object;

   Node *old_node     = current_node;
   current_node       = old_node->previous;

   if (back_node == old_node)
   {
      back_node = current_node;
      current_node->next = back_node;
   }
   else
      current_node->next = old_node->next;

   container_size--;

   delete old_node;

   return object;
}

template <typename Type>
Type *Container<Type>::pop_back (void)
{
   Node *desired_node = back_node;

   if (current_node == back_node)
      current_node = back_node->previous;

   back_node = back_node->previous;

   container_size--;

   return desired_node->object;
}

template <typename Type>
bool Container<Type>::advance_to_index (int index)
{
   if (index >= container_size)
   {
      std::cout << "requested index "
         << index
         << " exceeds the container size "
         << container_size << std::endl;
      return false;
   }

   // advance to the desired object node index
   while (current_index < index)
   {
      current_node = current_node->next;
      current_index++;
   }

   while (current_index > index)
   {
      current_node = current_node->previous;
      current_index--;
   }

   return true;
}

// Define container types for the action class
template void    Container<Action>::insert           (Action *action, int index);
template void    Container<Action>::push_front       (Action *action           );
template Action *Container<Action>::access           (int index                );
template Action *Container<Action>::pop              (int index                );
template Action *Container<Action>::pop_back         (void                     );
template bool    Container<Action>::advance_to_index (int index                );
