#ifndef POINT_CONVERSION_H
#define POINT_CONVERSION_H

float point_conversion(float  perspective[3],
                       float  direction[3],
                       float  window_distance,
                       float  point[3],
                       float *rot,
                       float *output_point);

void rotation( float  direction[3],
               float* rot );

#endif
