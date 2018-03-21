/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef QUAD_H_
#define QUAD_H_

#include "../object.h"
#include "./triangle.h"

/**
 * A Quadrangle is a polygon with 4 vertices in 3-dimensional space
 * Not all 4 points have to lie on the same plane, nor does it have to be convex
 */
class Quad: public Object {
    public:
        Quad(Point const &v0, Point const &v1, Point const &v2, Point const &v3);

        virtual Hit intersect(Ray const &ray);

    private:
        // A quadrangle consists of 2 triangles, which not necessarily lie on the same plane
        // If the quad is concave, the constructor will choose the triangles appropriately
        Triangle t1, t2;
};

#endif
