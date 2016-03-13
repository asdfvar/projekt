#include <list>
#include "assembly.h"
#include <iostream>

/*
** Constructor NAME: Assembly
*/
Assembly::Assembly ( void )
{
   center[0] = 0.0f;
   center[1] = 0.0f;
   center[2] = 0.0f;
}

/*
** Constructor NAME: Assembly
*/
Assembly::Assembly (float center_in[3])
{
   center[0] = center_in[0];
   center[1] = center_in[1];
   center[2] = center_in[2];
}

/*
** Function NAME: insert
*/
void Assembly::insert (Assembly *assembly_object)
{
   assembly_elements.push_back (*assembly_object);
}

/*
** Function NAME: insert
*/
void Assembly::insert (Sphere *sphere_object)
{
   sphere_elements.push_back (*sphere_object);
}

/*
** Function NAME: intersect
*/
bool Assembly::intersect (
    /* [I ] */     Ray     incomming_ray,
    /* [ O] */     Ray    *new_ray,
                   float  *normal,
    /* [ O] */     float  *distance,
                   float   color_intensities[],
                   float  *reflectivity,
                   float **reflection_table_x,
                   float **reflection_table_y,
                   int    *reflection_table_N)
{

   bool  intersection       = false;
   bool  first_intersection = true;
   float min_distance       = -1.0f;

   Ray   local_ray;
   float local_distance;

   float *local_reflection_table_x;
   float *local_reflection_table_y;
   int    local_reflection_table_N;
   float  local_color_intensities[3] = {0.0f, 0.0f, 0.0f};
   float  local_reflectivity;
   float  local_normal[3];

   /*
   ** Convert incoming ray to coordinates that are
   ** relative to the assembly.
   */
   float position[3] = { incomming_ray.get_position(0),
                         incomming_ray.get_position(1),
                         incomming_ray.get_position(2) };

   position[0] -= center[0];
   position[1] -= center[1];
   position[2] -= center[2];

   incomming_ray.set_position( position );

   /*
   ** Loop through the assemblies
   */

   bool intersect_this_assembly = false;

   for (std::list<Assembly>::iterator assembly_it = assembly_elements.begin();
        assembly_it != assembly_elements.end();
        assembly_it++)
   {
      intersect_this_assembly = assembly_it->intersect(  
                                                 incomming_ray,
                                                &local_ray,
                                                 local_normal,
                                                &local_distance,
                                                 local_color_intensities,
                                                &local_reflectivity,
                                                &local_reflection_table_x,
                                                &local_reflection_table_y,
                                                &local_reflection_table_N);

      if (intersect_this_assembly)
      {
         if (first_intersection || (local_distance < min_distance && local_distance > EPS))
         {
            first_intersection   = false;
            min_distance         = local_distance;
           *distance             = local_distance;
            color_intensities[0] = local_color_intensities[0];
            color_intensities[1] = local_color_intensities[1];
            color_intensities[2] = local_color_intensities[2];
           *reflectivity         = local_reflectivity;
           *new_ray              = local_ray;
            normal[0]            = local_normal[0];
            normal[1]            = local_normal[1];
            normal[2]            = local_normal[2];
           *reflection_table_x   = local_reflection_table_x;
           *reflection_table_y   = local_reflection_table_y;
           *reflection_table_N   = local_reflection_table_N;
         }
      }

      intersection |= intersect_this_assembly;

   }

   /*
   ** Loop through the spheres
   */

   bool intersect_this_sphere = false;

   for (std::list<Sphere>::iterator sphere_it = sphere_elements.begin();
        sphere_it != sphere_elements.end();
        sphere_it++)
   {
      intersect_this_sphere = sphere_it->intersect( incomming_ray,
                                                   &local_ray,
                                                    local_normal,
                                                   &local_distance,
                                                    local_color_intensities,
                                                   &local_reflectivity,
                                                   &local_reflection_table_x,
                                                   &local_reflection_table_y,
                                                   &local_reflection_table_N);

      if (intersect_this_sphere)
      {

         if (first_intersection || (local_distance < min_distance && local_distance > EPS))
         {
             first_intersection   = false;
             min_distance         = local_distance;
            *distance             = local_distance;
             color_intensities[0] = local_color_intensities[0];
             color_intensities[1] = local_color_intensities[1];
             color_intensities[2] = local_color_intensities[2];
           *reflectivity          = local_reflectivity;
            *new_ray              = local_ray;
             normal[0]            = local_normal[0];
             normal[1]            = local_normal[1];
             normal[2]            = local_normal[2];
            *reflection_table_x   = local_reflection_table_x;
            *reflection_table_y   = local_reflection_table_y;
            *reflection_table_N   = local_reflection_table_N;
         }
      }

      intersection |= intersect_this_sphere;

   }

   /*
   ** Loop through the planes
   */

   for (std::list<Sphere>::iterator sphere_it = sphere_elements.begin();
        sphere_it != sphere_elements.end();
        sphere_it++)
   {
   }


   /*
   ** Find if the new ray has a direct line of sight
   ** of the light source
   */

   /*
   ** Reset the position to the absolute coordinates
   ** (absolute from this assembly's perspective anyways).
   */

   position[0] += center[0];
   position[1] += center[1];
   position[2] += center[2];

   *distance = min_distance;
   if (!intersection) *new_ray = incomming_ray;

   return intersection;

}

/*
** Function NAME: intersect
*/
bool Assembly::intersect (
    /* [I ] */     Ray   incomming_ray,
                   float *distance)
{

   Ray new_ray;
   float color_intensities[3];
   float reflectivity;
   float *reflection_table_x;
   float *reflection_table_y;
   int    reflection_table_N;
   float  normal[3];

   return intersect (
              incomming_ray,
             &new_ray,
              normal,
              distance,
              color_intensities,
             &reflectivity,
             &reflection_table_x,
             &reflection_table_y,
             &reflection_table_N);
}
