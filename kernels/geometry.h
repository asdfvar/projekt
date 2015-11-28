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

bool intersect_triangle (
        /* [I ] */        float line_position[3],
        /* [I ] */        float line_direction[3],
        /* [I ] */        float triangle_pos_0[3],
        /* [I ] */        float triangle_pos_1[3],
        /* [I ] */        float triangle_pos_2[3],
        /* [ O] */        float *intersection,
        /* [ O] */        float *normal);
}

#endif
