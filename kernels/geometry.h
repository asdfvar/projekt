#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace geometry {

bool point_to_sphere (
               float position[3],
               float direction[3],
               float center[3],
               float radius,
               int   pick,
               float normal[3],
               float *intersection);

bool intersect_sphere (
               float position[3],
               float direction[3],
               float center[3],
               float radius);

}

#endif
