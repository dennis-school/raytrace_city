/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "quad.h"

#include <cmath>

using namespace std;

// Returns true if P is on the left of line v1-v0 which has normal N
static bool isLeftOf( Vector const &v0, Vector const &v1, Vector const &N, Vector const &P ) {
    Vector edge = v1 - v0;
    Vector v0p = P - v0;
    return N.dot( edge.cross( v0p ) ) >= 0;
}

// Returns true if the corner v0-v1-v2 is a corner in a clockwise-closing polygon
// parameter N is a reference normal
static bool isClockwise( Vector const &v0, Vector const &v1, Vector const &v2, Vector const &N ) {
    Vector N1 = ( v0 - v1 ).cross( v2 - v1 );
    if ( N1.dot( N ) < 0 )
        N1 = -N1;
    return isLeftOf( v2, v0, N1, v1 );
}

// Returns the index of the corner that is concave in the shape
// Returns -1 if no such corner exists
static int findConcaveIndex( const Point v[4] ) {
    Vector N0 = ( v[1] - v[0] ).cross( v[2] - v[0] );
    bool isClockwiseV[4];
    for ( int i = 0; i < 4; i++ )
        isClockwiseV[i] = isClockwise( v[(i+3)%4], v[i+0], v[(i+1)%4], N0 );
    // If 3 corners are clockwise, it is a clockwise quad with one concave corner
    // If 4 corners are clockwise, it is clearly a clockwise shape
    bool isClockwiseShape = ( isClockwiseV[0] + isClockwiseV[1] + isClockwiseV[2] + isClockwiseV[3] ) >= 3;
    for ( int i = 0; i < 4; i++ ) {
      if ( isClockwiseShape != isClockwiseV[i] )
        return i;
    }
    return -1;
}

Hit Quad::intersect(Ray const &ray) {
    Hit hit1 = t1.intersect( ray );
    Hit hit2 = t2.intersect( ray );
    if ( isnan( hit2.t ) || hit1.t < hit2.t )
        return hit1;
    return hit2;
}

Quad::Quad(Point const &v0, Point const &v1, Point const &v2, Point const &v3)
        : t1( v0, v1, v2 ), t2( v0, v2, v3 ) {
    // It is not necessarily a convex quad. Make sure the split happens at the appropriate vertex
    Point points[4] = { v0, v1, v2, v3 };
    int splitIndex = findConcaveIndex( points );
    // If there is no concave corner, it does not matter at which corner the split happens
    // So keep the ones assigned in the method initialisation. Otherwise make an explicit split
    if ( splitIndex != -1 ) {
        t1 = Triangle( points[ splitIndex ], points[ ( splitIndex + 1 ) % 4 ], points[ ( splitIndex + 2 ) % 4 ] );
        t2 = Triangle( points[ splitIndex ], points[ ( splitIndex + 2 ) % 4 ], points[ ( splitIndex + 3 ) % 4 ] );
    }
}
