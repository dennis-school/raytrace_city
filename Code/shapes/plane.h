/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef PLANE_H_
#define PLANE_H_

#include "../object.h"

/**
 * An infinite plane in 3d space. It is defined by a point on the plane and a normal.
 * The plane is visible on either side.
 */
class Plane: public Object
{
    public:
        Plane( Point const &point, Vector const &normal );

        virtual Hit intersect(Ray const &ray);

    private:
        Point point;
        Vector normal;
};

#endif
