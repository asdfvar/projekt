#ifndef ACTIONS_H
#define ACTIONS_H

/* Actions for units to take. These actions are defined as
 ** static in nature (usually performed on objects that
 ** don't move in the environment)
 */
class Action
{
   public:

      Action (int location_in[3], int type);

      bool is_complete (void) { return complete; };

   protected:

      int   type;        // type of action
      int   location[3]; // location the action takes place
      float duration;    // duration it takes to complete the action (seconds)
      float time_spent;  // total time spent working on the task
      bool  complete;    // flag to determine if the action is complete
};

class digAction : public Action
{
   public:

      digAction (int location_in[3], int type);
};

#endif
