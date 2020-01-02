#include "actions.h"

Action::Action (int location_in[3])
{
   location[0] = location_in[0];
   location[1] = location_in[1];
   location[2] = location_in[2];

   duration   = 0.0f;
   time_spent = 0.0f;
   complete   = false;
}

digAction::digAction (int location_in[3]) : Action (location_in)
{
   duration = 4.0f;
}
