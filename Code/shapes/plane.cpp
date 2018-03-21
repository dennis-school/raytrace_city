/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "plane.h"

#include <cmath>

using namespace std;

Hit Plane::intersect(Ray const &ray)
{
    Vector &N = normal;

    double NdotD = N.dot( ray.D );
    if ( NdotD == 0 ) {
        // The normal is orthogonal to the ray, meaning the triangle's plane does not intersect with the ray
        return Hit::NO_HIT( );
    }

    double originDistance = N.dot( point );

    double t = ( originDistance - N.dot( ray.O ) ) / NdotD;
    N.normalize( );

    if ( t <= 0 )
        // The triangle is behind the ray's origin (or equal to - easier for later bounce tracing)
        return Hit::NO_HIT( );

    if ( NdotD > 0 ) {
        // Pick the normal that points towards the ray origin, so that it is visible from both sides
        N = -N;
    }

    return Hit(t,N);
}

Plane::Plane( Point const &point, Vector const &normal )
:
    point( point ),
    normal( normal ) // Assume here the normal is normalized (i.e. it is unit size)
{}
