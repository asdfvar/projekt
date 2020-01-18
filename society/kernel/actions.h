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

class Container
{
   public:

      Container (void)
      {
         container_size = 0;
         current_index  = 0;
      }

      void insert (Action *action, int index);

      Action *access (int index);

      Action *pop (int index);

   private:

      int container_size;
      // first index pointer

      int   current_index;
      Node *current_node;

      bool advance_to_index (int index);
};

#endif
