/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "cylinder.h"

#include <cmath>

using namespace std;

Hit Cylinder::intersect( Ray const &ray ) {
    // It is a cylinder along the y-axis. The cylinder is essentially a 2d-circle, when seen from top-view
    // Essentially an intersection with an infinite cylinder along the y-axis is computed, by taking
    // the intersection with the 2d-circle in the xz-plane. Which is then bounded on the y-axis

    Point origin2d = Vector( ray.O.x, 0, ray.O.z );
    Vector direction2d = Vector( ray.D.x, 0, ray.D.z );
    double direction2dLen = direction2d.length( );
    direction2d.normalize( );
    Vector position2d = Vector( position.x, 0, position.z );

    // Solve: ((O-P)+D*t)_{xz}^2 - R_{xz}^2
    double a = 1; // Dxz^2
    double b = 2 * direction2d.dot( origin2d - position2d );
    double c = ( origin2d - position2d ).dot( origin2d - position2d ) - radius*radius;
    double D = b * b - 4 * a * c;

    if ( D < 0 )
        return Hit::NO_HIT( );

    double t0 = ( -b + sqrt( D ) ) / ( 2 * a );
    double t1 = ( -b - sqrt( D ) ) / ( 2 * a );
    bool hitsBothSides = ( t0 >= 0 && t1 >= 0 );

    double t = min( t0, t1 );
    double otherT = max( t0, t1 );
    if ( t < 0 ) {
        double tempT = t;
        t = otherT;
        otherT = tempT;

        if ( t < 0 ) // The cylinder is fully behind the "camera"
            return Hit::NO_HIT( );
    }

    Vector P = ray.O + ray.D * t / direction2dLen;

    if ( P.y < position.y || P.y > position.y + height ) {
        // It certainly does not hit one of the sides. May still hit the bottom

        Vector otherP = ray.O + ray.D * otherT / direction2dLen;  // Hit on the back side

        // This is sort of a cheat. If the cylinder were open on the top and bottom, the "back" side would be visible from within
        //   If this back side is visible from within, instead replace it with the top or bottom "lid".
        if ( hitsBothSides && ( otherP.y >= position.y && otherP.y <= position.y + height ) && ray.D.y != 0 ) {
            if ( P.y > position.y + height ) {
                t = ( position.y + height - ray.O.y ) / ray.D.y;
                return Hit(t,Vector(0,1,0));
            } else if ( P.y < position.y ) {
                t = ( position.y - ray.O.y ) / ray.D.y;
                return Hit(t,Vector(0,-1,0));
            }
        }
        return Hit::NO_HIT( );
    }

    Vector P2d = Vector( P.x, 0, P.z );
    Vector N = ( P2d - position2d ) / radius;

    return Hit(t,N);
}

Cylinder::Cylinder( Point const &position, double height, double radius )
:
    position(position),
    height(height),
    radius(radius)
{
}
