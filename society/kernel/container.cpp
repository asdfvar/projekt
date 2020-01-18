#include "container.h"
#include "actions.h"
#include <iostream>

template <typename Type>
void Container<Type>::insert (Type *object, int index)
{
   // advance to the desired object node index
   bool stat = advance_to_index (index);
   if (!stat) return;

   Node *node = new Node;
   node->object = object;

   // Attach the new node
   node->previous = current_node->previous;
   node->next     = current_node;

   // Configure the linkage for neighboring nodes
   current_node->previous = node;
   node->previous->next = node;

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

   Type *select_action = current_node->object;

   // Effectively pop the node off
   Node *next_node = current_node->next;
   current_node = current_node->previous;
   current_node->next = next_node;

   container_size--;

   return select_action;
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
template Action *Container<Action>::access           (int index                );
template Action *Container<Action>::pop              (int index                );
template bool    Container<Action>::advance_to_index (int index                );
