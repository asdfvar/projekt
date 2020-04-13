#include "jobs.h"
#include "item.h"
#include <iostream>

Job::Job (int flattened_index_in, int location_in[3], int type_in, unsigned int material_in)
{
   type = type_in; // TODO: have the type be implicit by the *subclass*
   material = material_in;

   flattened_loc_index = flattened_index_in;

   location[0] = location_in[0];
   location[1] = location_in[1];
   location[2] = location_in[2];

   energy     = 0.2f;
   work       = 0.0f;
   complete   = false;

   if      (material == tid::stone) energy = 0.4f;
   else if (material == tid::dirt)  energy = 0.2f;
   else if (material == tid::grass) energy = 0.25f;
}

void Job::act (float Qin)
{
   work += Qin;

   if (work >= energy) complete = true;
}
