#include "ray.h"

Ray::Ray( float position_in[3],
          float direction_in[3],
          float intensity_in)
{

   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];

   direction[0] = direction_in[0];
   direction[1] = direction_in[1];
   direction[2] = direction_in[2];

   intensity = intensity_in;

}
