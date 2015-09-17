#include "light_source.h"

/*
** Constructor NAME: Light_source
*/
Light_source::Light_source( float position_in[3],
                            float intensity_in[3])
{
   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];

   intensity[0] = intensity_in[0];
   intensity[1] = intensity_in[1];
   intensity[2] = intensity_in[2];
}

Light_source::~Light_source() {};
