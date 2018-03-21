/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "cone.h"

#include <cmath>

using namespace std;

// Computes the dot product between 'a' and 'b' while disregarding the y-component
// So, essentially, it is considered as a 2d vector from top view
double dotXZ( const Vector& a, const Vector& b ) {
    return a.x * b.x + a.z * b.z;
}

// Computes the dot product of 'a' with itself, while disregarding the y-component
// So, essentially, it is considered as a 2d vector from top view
double dotXZ( const Vector& a ) {
    return dotXZ( a, a );
}

Hit Cone::intersect( Ray const &ray ) {
    // A cone is defined by x^2 + z^2 = y^2  for 0 <= y <= 1

    // It is a cone along the y-axis. When seen from top-view the cone is essentially a 2d-circle
    // that gets smallar as it reaches its top. Intuitively, the intersection with this "shrinking circle"
    // is computed along the xz-axes; While it shrinks along y. Then it is bounded between its bounds
    // defined along the y-axis

    Vector topPosition = this->position + Vector( 0, height, 0 );

    double slope2 = ( radius / height ) * ( radius / height );
    double a = dotXZ( ray.D ) - slope2; // Dxz^2
    double b = 2 * ( dotXZ( ray.D, ray.O - topPosition ) - ray.D.dot( ray.O - topPosition ) * slope2 );
    double c = dotXZ( ray.O - topPosition ) - ( ray.O - topPosition ).length_2( ) * slope2;
    double D = b * b - 4 * a * c;

    if ( D < 0 )
        return Hit::NO_HIT( );

    double t0 = ( -b + sqrt( D ) ) / ( 2 * a );
    double t1 = ( -b - sqrt( D ) ) / ( 2 * a );
    bool hitsBothSides = ( t0 > 0 && t1 > 0 );

    double t = min( t0, t1 );
    double otherT = max( t0, t1 );
    if ( t <= 0 ) {
        double tempT = t;
        t = otherT;
        otherT = tempT;

        if ( t <= 0 ) // The cone is fully behind the "camera"
            return Hit::NO_HIT( );
    }

    Vector P = ray.O + ray.D * t;
    Vector otherP = ray.O + ray.D * otherT; // Hit on the back side

    if ( P.y < position.y && hitsBothSides && ( otherP.y >= position.y && otherP.y <= position.y + height ) && ray.D.y != 0 ) {
        // It certainly does not hit one of the sides. May still hit the bottom

        // This is sort of a cheat. If the cone were open on the bottom, the "back" side would be visible from within
        //   If this back side is visible from within, instead replace it with the bottom "lid".
        t = ( position.y - ray.O.y ) / ray.D.y;
        return Hit(t,Vector(0,-1,0));
    } else if ( P.y < position.y || P.y > position.y + height ) {
        return Hit::NO_HIT( );
    }

    Vector P2d = Vector( P.x, 0, P.z );
    Vector N = ( P2d - Vector( position.x, 0, position.z ) ).normalized( );
    N = Vector( N.x, radius / height, N.z ).normalized( );

    return Hit(t,N);
}

Cone::Cone( Point const &position, double height, double radius )
:
    position(position),
    height(height),
    radius(radius)
{
}
