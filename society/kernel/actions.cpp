#include "actions.h"

Action::Action (int location_in[3], int type_in)
{
   type = type_in;

   location[0] = location_in[0];
   location[1] = location_in[1];
   location[2] = location_in[2];

   duration   = 4.0f;
   time_spent = 0.0f;
   complete   = false;
}

digAction::digAction (int location_in[3], int type_in) : Action (location_in, type_in)
{
   duration = 4.0f;
}
