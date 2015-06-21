#include "shape.h"

sphere::sphere ( float center_in[3],
                 float radius_in )
               : shape (center_in)
{
   radius = radius_in;
}
