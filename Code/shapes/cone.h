/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef CONE_H_
#define CONE_H_

#include "../object.h"

/**
 * A cone has a position at which its base is located. At the base the radius of the cone is at its maximum.
 * At the top of the cone, along the y-axis, the radius (within the xz-plane) is 0. The cone is closed at
 * the bottom.
 */
class Cone: public Object
{
    public:
        Cone( Point const &position, double height, double radius );

        virtual Hit intersect(Ray const &ray);

    private:
        // The base of the cone
        Point position;
        // The height along the y-axis
        double height;
        // The radius of the cone at the base
        double radius;
};

#endif
