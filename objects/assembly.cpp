#include <list>
#include "assembly.h"

Assembly::Assembly (float center_in[3])
{
   center[0] = center_in[0];
   center[1] = center_in[1];
   center[2] = center_in[2];
}

void Assembly::insert (Assembly assembly_object)
{
   assembly_elements.push_back (assembly_object);
}

void Assembly::insert (Sphere sphere_object)
{
   sphere_elements.push_back (sphere_object);
}

bool Assembly::intersect (Ray    incomming_ray,
                          Ray   *new_ray,
                          float *distance)
{

   bool  intersection = false;
   bool  first_intersection = true;
   float min_distance = -1.0f;

   Ray local_ray;
   float local_distance;

   /*
   ** Convert incoming ray to coordinates that are
   ** relative to the assembly.
   */

   float position[3] = {incomming_ray.get_position(0),
                        incomming_ray.get_position(1),
                        incomming_ray.get_position(2)};

   position[0] -= center[0];
   position[1] -= center[1];
   position[2] -= center[2];

   incomming_ray.set_position( position );

   /*
   ** Loop through the assemblies
   */

   /*
   ** Loop through the spheres
   */

   bool intersect_this_sphere = false;

   for (std::list<Sphere>::iterator sphere_it = sphere_elements.begin();
        sphere_it != sphere_elements.end();
        sphere_it++)
   {
      intersect_this_sphere |= sphere_it->intersect(  incomming_ray,
                                                     &local_ray,
                                                     &local_distance);
      if (intersect_this_sphere)
      {
         if (first_intersection)
         {
             first_intersection = false;
             min_distance = local_distance;
            *distance = local_distance;
            *new_ray = local_ray;
         }
      }
   }

   intersection |= intersect_this_sphere;

   /*
   ** Reset the position to the absolute coordinates
   */

   position[0] += center[0];
   position[1] += center[1];
   position[2] += center[2];

   *distance = min_distance;

   return intersection;

}
