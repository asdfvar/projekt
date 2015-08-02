#include <list>
#include "assembly.h"

Assembly::Assembly (float center_in[3])
{
   center[0] = center_in[0];
   center[1] = center_in[1];
   center[2] = center_in[2];
}

void Assembly::insert (Sphere sphere_object)
{
   sphere_elements.push_back (sphere_object);
}

bool Assembly::intersect (Ray ray)
{

   bool intersection = false;

   /*
   ** Convert incoming ray to coordinates that are
   ** relative to the assembly.
   */

   float position[3] = {ray.get_position(0),
                        ray.get_position(1),
                        ray.get_position(2)};

   position[0] -= center[0];
   position[1] -= center[1];
   position[2] -= center[2];

   ray.set_position( position );

   /*
   ** Loop through the spheres
   */

   for (std::list<Sphere>::iterator sphere_it = sphere_elements.begin();
        sphere_it != sphere_elements.end();
        sphere_it++)
   {
      intersection |= sphere_it->intersect( ray );
   }

   /*
   ** Loop through the ???
   */

   return intersection;

}
