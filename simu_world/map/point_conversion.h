#ifndef POINT_CONVERSION_H
#define POINT_CONVERSION_H

void point_conversion(float  perspective[3],
                      float  direction[3],
                      float  window_distance,
                      float  point[3],
                      float *output_point);

#endif
