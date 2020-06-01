#include "container.h"
#include "jobs.h"
#include "item.h"
#include "unit.h"
#include "map.h"
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
   if (!stat) {
      std::cout << "advancement error, returning null" << std::endl;
      return nullptr;
   }

   return current_node->object;
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

   template <typename Type>
bool Container<Type>::test_duplicates (void)
{
   bool duplicate = false;

   Node *node1 = front_node;
   Node *node2 = front_node;
   
   while (node1 != back_node) {
      node2 = node1;
      while (node2 != back_node) {
         node2 = node2->next;
         if (node1->object == node2->object) {
            std::cout << "duplicate found: node1 object = "
               << node1->object << " = " << node2->object
               << " = node2 object" << std::endl;
            duplicate = true;
         }
      }
   }

   return duplicate;
}

   template <typename Type>
bool Container<Type>::test_nulls (void)
{
   bool nulls = false;
   Node *node = front_node;

   while (node != back_node) {
      if (node->object == nullptr) nulls = true;
      node = node->next;
   }

   return nulls;
}

   template <typename Type>
Lattice<Type>::Lattice (int *size_in, int dim_in)
{
   dim = dim_in;

   size          = new int[dim];
   current_index = new int[dim];

   for (int ind = 0; ind < dim; ind++) size[ind]          = size_in[ind];
   for (int ind = 0; ind < dim; ind++) current_index[ind] = 0;

   // Build the lattice
   int total_size = 1;
   for (int ind = 0; ind < dim; ind++) total_size *= size[ind];

   Node *node = new Node;

   first_node = node;
   first_node->flat_previous = first_node;

   node->object = new Type;

   node->next     = new Node*[dim];
   node->previous = new Node*[dim];

   // String together all the nodes flattened over one dimension
   for (int ind = 1; ind < total_size; ind++)
   {
      node->flat_next = new Node;
      Node *l_node   = node;
      node = node->flat_next;
      node->flat_previous = l_node;
      node->object        = new Type;

      node->next     = new Node*[dim];
      node->previous = new Node*[dim];
   }

   last_node            = node;
   last_node->flat_next = last_node;

   // Build the connections of the lattice to connect adjacent cells
   int dist = 1;
   for (int rank = 0; rank < dim; rank++)
   {
      Node *node = first_node;
      if (rank > 0) dist *= size[rank - 1];

      bool done = false;
      while (!done)
      {
         Node *inode = node;

         for (int ind = 0; ind < dist; ind++) inode = inode->flat_next;

         node->next[rank] = inode;

         inode = node;

         for (int ind = 0; ind < dist; ind++) inode = inode->flat_previous;

         node->previous[rank] = inode;

         if (node == last_node) done = true;
         node = node->flat_next;
      }
   }

   // Set the current node
   current_node = first_node;
}

   template <typename Type>
Lattice<Type>::~Lattice (void)
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

   template <typename Type>
Type *Lattice<Type>::access (int *index)
{
   for (int ind = 0; ind < dim; ind++)
   {
      if (index[ind] < 0 || index[ind] >= size[ind])
         return nullptr;

      while (current_index[ind] < index[ind])
      {
         current_node = current_node->next[ind];
         current_index[ind]++;
      }

      while (current_index[ind] > index[ind])
      {
         current_node = current_node->previous[ind];
         current_index[ind]--;
      }
   }

   return current_node->object;
}

// Define container types for the job class
template void  Container<Job>::insert           (Job*, int);
template void  Container<Job>::push_front       (Job*     );
template void  Container<Job>::push_back        (Job*     );
template Job  *Container<Job>::access           (int      );
template Job  *Container<Job>::back             (void     );
template Job  *Container<Job>::pop              (int      );
template Job  *Container<Job>::pop_back         (void     );
template void  Container<Job>::reset            (void     );
template bool  Container<Job>::advance          (int      );
template void  Container<Job>::list_contents    (void     );
template void  Container<Job>::test_ends        (void     );
template bool  Container<Job>::test_duplicates  (void     );
template bool  Container<Job>::test_nulls       (void     );

// Define container types for the unit class
template void  Container<Unit>::insert          (Unit*, int);
template void  Container<Unit>::push_front      (Unit*     );
template void  Container<Unit>::push_back       (Unit*     );
template Unit *Container<Unit>::access          (int       );
template Unit *Container<Unit>::back            (void      );
template Unit *Container<Unit>::pop             (int       );
template Unit *Container<Unit>::pop_back        (void      );
template void  Container<Unit>::reset           (void      );
template bool  Container<Unit>::advance         (int       );
template void  Container<Unit>::list_contents   (void      );
template void  Container<Unit>::test_ends       (void      );
template bool  Container<Unit>::test_duplicates (void      );
template bool  Container<Unit>::test_nulls      (void      );

// Define container types for the items class
template void  Container<Item>::insert          (Item*, int);
template void  Container<Item>::push_front      (Item*     );
template void  Container<Item>::push_back       (Item*     );
template Item *Container<Item>::access          (int       );
template Item *Container<Item>::back            (void      );
template Item *Container<Item>::pop             (int       );
template Item *Container<Item>::pop_back        (void      );
template void  Container<Item>::reset           (void      );
template bool  Container<Item>::advance         (int       );
template void  Container<Item>::list_contents   (void      );
template void  Container<Item>::test_ends       (void      );
template bool  Container<Item>::test_duplicates (void      );
template bool  Container<Item>::test_nulls      (void      );

// Lattice for the Cell type
template Lattice<Cell>::Lattice (int *size_in, int dim_in);
template Lattice<Cell>::~Lattice (void);
template Cell *Lattice<Cell>::access (int *index);
