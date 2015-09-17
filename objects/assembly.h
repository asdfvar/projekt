#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <list>
#include "ray.h"
#include "shape.h"
#include "sphere.h"

/*
** Class NAME: Assembly
*/
class Assembly
{

   public:

      Assembly ( void );
      Assembly (float center[3]);

      float center[3];

      void insert (Assembly assembly_object);
      void insert (Sphere   sphere_object);

      bool intersect(
                  Ray    incomming_ray,
                  Ray   *new_ray,
                  float *distance);

   private:

      std::list<Sphere>   sphere_elements;
      std::list<Assembly> assembly_elements;

};

#endif
