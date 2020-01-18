#include "actions.h"
#include <iostream>

Action::Action (int flattened_index_in, int location_in[3], int type_in)
{
   type = type_in;

   flattened_index = flattened_index_in;

   location[0] = location_in[0];
   location[1] = location_in[1];
   location[2] = location_in[2];

   duration   = 4.0f;
   time_spent = 0.0f;
   complete   = false;
}

template <typename Type>
void Container<Type>::insert (Type *object, int index)
{
   // advance to the desired object node index
   bool stat = advance_to_index (index);
   if (!stat) return;

   Node<Type> *node = new Node<Type>;
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
   Node<Type> *next_node = current_node->next;
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

template void Container<Action>::insert (Action *action, int index);
template Action *Container<Action>::access (int index);
template Action *Container<Action>::pop (int index);
template bool Container<Action>::advance_to_index (int index);
