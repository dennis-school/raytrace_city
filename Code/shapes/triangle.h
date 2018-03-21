/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

/**
 * A triangle is defined by 3 points in 3d space. By definition, these 3 points lie on a plane in 3d.
 */
class Triangle: public Object
{
    public:
        Triangle(Point const &v0, Point const &v1, Point const &v2 );

        virtual Hit intersect(Ray const &ray);

    private:
        // These points are always defined clockwise, along their natural normal
        // (this is assured by the constructor)
        Point v0, v1, v2;
};

#endif
