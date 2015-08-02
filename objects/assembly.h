#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <list>
#include "ray.h"
#include "shape.h"
#include "sphere.h"

class Assembly
{

   public:

      Assembly (float center[3]);

      float center[3];

      void insert (Sphere sphere_object);

      bool intersect( Ray );

   private:

      std::list<Sphere> sphere_elements;

};

#endif
