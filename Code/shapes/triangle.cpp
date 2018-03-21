/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "triangle.h"

#include <cmath>
#include <limits>

using namespace std;

// Returns true if P is on the left of line v1-v0 which has normal N
static bool isLeftOf( Point const &v0, Point const &v1, Vector const &N, Point const &P ) {
    Vector edge = v1 - v0;
    Vector v0p = P - v0;
    return N.dot( edge.cross( v0p ) ) >= 0;
}

// Returns true if P is on the left of line v1-v0 which has normal N
// This function is necessary to ensure no gaps (T-junctions) occur between adjacent triangles.
static bool isApproxLeftOf( Point const &v0, Point const &v1, Vector const &N, Point const &P ) {
    Vector edge = v1 - v0;
    Vector v0p = P - v0;
    return N.dot( edge.cross( v0p ) ) + std::numeric_limits< double >::epsilon( ) >= 0;
}


Hit Triangle::intersect(Ray const &ray)
{
    Vector N = ( v1 - v0 ).cross( v2 - v0 );

    double NdotD = N.dot( ray.D );
    if ( NdotD == 0 ) {
        // The normal is orthogonal to the ray, meaning the triangleś plane does not intersect with the ray
        return Hit::NO_HIT( );
    }

    double originDistance = N.dot( v0 );

    double t = ( originDistance - N.dot( ray.O ) ) / NdotD;
    N.normalize( );

    if ( t <= 0 )
        // The triangle is behind the ray's origin
        return Hit::NO_HIT( );

    Vector P = ray.at( t );

    if ( isApproxLeftOf( v0, v1, N, P ) && isApproxLeftOf( v1, v2, N, P ) && isApproxLeftOf( v2, v0, N, P ) ) {
        if ( NdotD > 0 ) {
            // Pick the normal that points towards the ray origin, so that it is visible from both sides
            N = -N;
        }
        return Hit(t,N);
    } else {
        return Hit::NO_HIT( );
    }
}

Triangle::Triangle(Point const &v0, Point const &v1, Point const &v2 )
:
    v0(v0),
    v1(v1),
    v2(v2)
{
    // Make sure the triangle is clockwise around its natural normal
    Vector N = ( v1 - v0 ).cross( v2 - v0 );
    if ( !isLeftOf( v0, v1, N, v2 ) ) {
        Point const vT = this->v0;
        this->v0 = this->v2;
        this->v2 = vT;
    }
}
