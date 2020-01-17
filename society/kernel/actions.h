#ifndef ACTIONS_H
#define ACTIONS_H

/* Actions for units to take. These actions are defined as
 ** static in nature (usually performed on objects that
 ** don't move in the environment)
 */
class Action
{
   public:

      Action (int flattened_index, int location_in[3], int type);

      bool is_complete (void) { return complete; };

      int get_position (int ind) { return location[ind]; };

      int get_flattened_index (void) { return flattened_index; };

   protected:

      int   type;            // type of action
      int   location[3];     // location the action takes place
      int   flattened_index; // index (flattened location)
      float duration;        // duration it takes to complete the action (seconds)
      float time_spent;      // total time spent working on the task
      bool  complete;        // flag to determine if the action is complete
};

class Node
{
   public:

      Action *action;
      Node   *next;
      Node   *previous;
};

class Actions_container
{
   public:

      Actions_container (void)
      {
         container_size       = 0;
         current_action_index = 0;
      }

      // insert action (index)
      void insert_action (Action *action, int index)
      {
         // advance to the desired action node index
         while (current_action_index < index)
         {
            current_action_node = current_action_node->next;
            current_action_index++;
         }

         while (current_action_index > index)
         {
            current_action_node = current_action_node->previous;
            current_action_index--;
         }

         // TODO: logic to insert
      }

      // access action (index)

      // pop action (index) // returns address to the select action and removes it from the list
      Action *pop (int index)
      {
         if (container_size == 0) return nullptr;

         // advance to the desired action node index
         while (current_action_index < index)
         {
            current_action_node = current_action_node->next;
            current_action_index++;
         }

         while (current_action_index > index)
         {
            current_action_node = current_action_node->previous;
            current_action_index--;
         }

         Action *select_action = current_action_node->action;

         // Effectively pop the node off
         Node *next_node = current_action_node->next;
         current_action_node = current_action_node->previous;
         current_action_node->next = next_node;

         container_size--;

         return select_action;
      }

   private:

      int container_size;
      // first index pointer

      int   current_action_index;
      Node *current_action_node;
};

#endif
