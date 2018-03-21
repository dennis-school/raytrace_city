/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "../object.h"

/**
 * A cylinder has a position at which its base is located. The cylinder "grows" along the y-axis.
 * Its radius is a circle within the xz-plane. The cylinder is closed on its top and bottom.
 */
class Cylinder: public Object
{
    public:
        Cylinder( Point const &position, double height, double radius );

        virtual Hit intersect(Ray const &ray);

    private:
        // The base of the cylinder
        Point position;
        // The height along the y-axis
        double height;
        // The radius of the cylinder
        double radius;
};

#endif
