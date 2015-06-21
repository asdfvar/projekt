#include "assembly.h"

Assembly::Assembly (float center_in[3])
{
   center[0] = center_in[0];
   center[1] = center_in[1];
   center[2] = center_in[2];
}

void Assembly::insert (sphere *sphere_object)
{
   elements.push_back (sphere_object);
}
