#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace geometry {

int point_to_sphere (float p[3],
                     float u[3],
                     float c[3],
                     float *v);

bool intersect_sphere (
               float p[3],
               float u[3],
               float center[3],
               float radius);

}

#endif
