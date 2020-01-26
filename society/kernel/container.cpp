#include "container.h"
#include "jobs.h"
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
      front_node    = back_node = node;
      node->next    = node->previous = node;
      current_node  = node;
      current_index = 0;
   }
   else
   {
      // advance to the desired object node index
      bool stat = advance (index);
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
Type *Container<Type>::pop (int index)
{
   if (container_size == 0) return nullptr;

   // advance to the desired object node index
   bool stat = advance (index);
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

   if (current_index >= container_size) current_index = container_size - 1;

   Type *object = old_node->object;

   delete old_node;

   return object;
}

template <typename Type>
Type *Container<Type>::pop (void)
{
   Type *object = pop (current_index);
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
Type *Container<Type>::access (int index)
{
   // advance to the desired object node index
   bool stat = advance (index);
   if (!stat) return nullptr;

   return current_node->object;
}

template <typename Type>
Type *Container<Type>::access (void)
{
   return current_node->object;
}

template <typename Type>
void Container<Type>::advance (void)
{
   if (current_node == back_node)
   {
      current_node = front_node;
      current_index = 0;
   }
   else
   {
      current_node = current_node->next;
      current_index++;
   }
}

template <typename Type>
bool Container<Type>::advance (int index)
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
      advance (ind);
      std::cout << current_node->object << ", ";
   }
   std::cout << std::endl;

   std::cout << "back     = " << back_node->object << std::endl;
   std::cout << std::endl;
}

template <typename Type>
void Container<Type>::test_ends (void)
{
   advance (0);
   if (front_node != current_node)
   {
      std::cout << "front node " << front_node->object
         << " not defined " << current_node->object
         << std::endl;
   }

   advance (container_size - 1);
   if (back_node != current_node)
   {
      std::cout << "back node " << back_node->object
         << " not defined " << current_node->object
         << std::endl;
   }
}

// Define container types for the job class
template void  Container<Job>::insert           (Job*, int);
template void  Container<Job>::push_front       (Job*     );
template void  Container<Job>::push_back        (Job*     );
template Job  *Container<Job>::access           (int      );
template Job  *Container<Job>::access           (void     );
template Job  *Container<Job>::back             (void     );
template Job  *Container<Job>::pop              (int      );
template Job  *Container<Job>::pop              (void     );
template Job  *Container<Job>::pop_back         (void     );
template void  Container<Job>::reset            (void     );
template void  Container<Job>::advance          (void     );
template bool  Container<Job>::advance          (int      );
template void  Container<Job>::list_contents    (void     );
template void  Container<Job>::test_ends        (void     );

// Define container types for the unit class
template void  Container<Unit>::insert          (Unit*, int);
template void  Container<Unit>::push_front      (Unit*     );
template void  Container<Unit>::push_back       (Unit*     );
template Unit *Container<Unit>::access          (int       );
template Unit *Container<Unit>::access          (void      );
template Unit *Container<Unit>::back            (void      );
template Unit *Container<Unit>::pop             (int       );
template Unit *Container<Unit>::pop             (void      );
template Unit *Container<Unit>::pop_back        (void      );
template void  Container<Unit>::reset           (void      );
template void  Container<Unit>::advance         (void      );
template bool  Container<Unit>::advance         (int       );
template void  Container<Unit>::list_contents   (void      );
template void  Container<Unit>::test_ends       (void      );
