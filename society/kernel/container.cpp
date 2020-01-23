#include "container.h"
#include "actions.h"
#include "unit.h"
#include <iostream>

template <typename Type>
void Container<Type>::push_front (Type *object)
{
   current_node = front_node;
   current_index = 0;

   insert (object, 0);
}

template <typename Type>
void Container<Type>::push_back (Type *object)
{
   current_node = back_node;
   current_index = container_size - 1;

   insert (object, container_size - 1);
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
   {
      node->next             = current_node;
      node->previous         = node;
      current_node->previous = node;
      front_node             = node;
   }

   else
   {
      // Attach the new node
      node->previous = current_node->previous;
      node->next     = current_node;

      // Configure the linkage for neighboring nodes
      current_node->previous = node;
      node->previous->next   = node;
   }

   // The current node becomes the new node
   current_node = node;

   container_size++;
}

template <typename Type>
Type *Container<Type>::back (void)
{
   return back_node->object;
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

   Node *old_node = current_node;

   if (old_node == front_node)
   {
      current_node           = old_node->next;
      current_node->next     = old_node->next->next;
      current_node->previous = current_node;
      front_node             = current_node;
   }

   else if (old_node == back_node)
   {
      current_node       = old_node->previous;
      current_node->next = current_node;
      back_node          = current_node;
   }

   else //if (old_node != front_node && old_node != back_node)
   {
      current_node             = old_node->next;
      old_node->previous->next = old_node->next;
      old_node->next->previous = old_node->previous;
   }

   container_size--;

   Type *object = old_node->object;

   delete old_node;

   return object;
}

template <typename Type>
Type *Container<Type>::pop_back (void)
{
   Node *desired_node = back_node;

   if (current_node == back_node)
   {
      current_node = back_node->previous;
      current_index--;
      if (current_index < 0) current_index = 0;
   }

   back_node = back_node->previous;

   back_node->next = back_node;

   container_size--;

   return desired_node->object;
}

template <typename Type>
void Container<Type>::reset (void)
{
   container_size = 0;
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

template <typename Type>
void Container<Type>::list_contents (void)
{

   std::cout << "front    = " << front_node->object << std::endl;

   std::cout << "contents = ";
   for (int ind = 0; ind < container_size; ind++)
   {
      advance_to_index (ind);
      std::cout << current_node->object << ", ";
   }
   std::cout << std::endl;

   std::cout << "back     = " << back_node->object << std::endl;
   std::cout << std::endl;
}

template <typename Type>
void Container<Type>::test_ends (void)
{
   advance_to_index (0);
   if (front_node != current_node) std::cout << "front node " << front_node->object << " not defined " << current_node->object << std::endl;

   advance_to_index (container_size - 1);
   if (back_node != current_node) std::cout << "back node " << back_node->object << " not defined " << current_node->object << std::endl;
}

// Define container types for the action class
template void    Container<Action>::insert           (Action*, int);
template void    Container<Action>::push_front       (Action*     );
template void    Container<Action>::push_back        (Action*     );
template Action *Container<Action>::access           (int         );
template Action *Container<Action>::back             (void        );
template Action *Container<Action>::pop              (int         );
template Action *Container<Action>::pop_back         (void        );
template void    Container<Action>::reset            (void        );
template bool    Container<Action>::advance_to_index (int         );
template void    Container<Action>::list_contents    (void        );
template void    Container<Action>::test_ends        (void        );

// Define container types for the unit class
template void  Container<Unit>::insert           (Unit*, int);
template void  Container<Unit>::push_front       (Unit*     );
template void  Container<Unit>::push_back        (Unit*     );
template Unit *Container<Unit>::access           (int       );
template Unit *Container<Unit>::back             (void      );
template Unit *Container<Unit>::pop              (int       );
template Unit *Container<Unit>::pop_back         (void      );
template void  Container<Unit>::reset            (void      );
template bool  Container<Unit>::advance_to_index (int       );
template void  Container<Unit>::list_contents    (void      );
template void  Container<Unit>::test_ends        (void      );
