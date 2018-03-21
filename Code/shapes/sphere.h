/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"

/**
 * A sphere is defined by a position and a radius. Any point that has a distance of "radius"
 * from the position is a point on the sphere.
 */
class Sphere: public Object
{
    public:
        Sphere(Point const &pos, double radius, Rotation const &rotation);

        virtual Hit intersect(Ray const &ray);
        virtual Point2 uvMap( Point p );

        Point const position;
        double const r;
        Rotation rotation;
};

#endif
