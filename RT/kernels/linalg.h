#ifndef LINALG_H
#define LINALG_H

namespace linalg
{

float summation (float *x,
                 int    N);

float dot_product (float *a,
                   float *b,
                   int   N);

float norm_squared (float *a,
                    int    N);

float norm (float *a,
            int    N);

void unit_vector (float *u,
                  int    N);

void project (float *a,
              float *b,
              float *c,
              int    N);

}

#endif
