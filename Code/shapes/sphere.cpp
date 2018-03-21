/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "sphere.h"

#include <cmath>

using namespace std;

void rotate( double &x, double &y, double angle );

Hit Sphere::intersect(Ray const &ray)
{
    // Using algebraic solution. (Non-geometric)
    // Solve: ((O-P)+D*t)^2 - R^2
    double a = 1; // D^2
    double b = 2 * ray.D.dot( ray.O - position );
    double c = ( ray.O - position ).dot( ray.O - position ) - r*r;
    double D = b * b - 4 * a * c;

    if ( D < 0 )
        return Hit::NO_HIT( );

    double t0 = ( -b + sqrt( D ) ) / ( 2 * a );
    double t1 = ( -b - sqrt( D ) ) / ( 2 * a );

    double t = min( t0, t1 );
    if ( t <= 0 ) {
        t = max( t0, t1 );

        if ( t <= 0 ) // The sphere is fully behind the "camera"
            return Hit::NO_HIT( );
    }

    Vector N = ( ray.at( t ) - position ) / r;

    return Hit(t,N);
}

Point2 Sphere::uvMap( Point p ) {
    double x = ( p.x - position.x ) / r;
    double y = ( p.y - position.y ) / r;
    double z = ( p.z - position.z ) / r;

    // Rotate about the x-axis
    rotate( y, z, rotation.x );

    // Rotate about the y-axis
    rotate( z, x, rotation.y );
    
    // Rotate about the z-axis
    rotate( x, y, rotation.z );

    double u = 0.5 + atan2( x, z ) / ( 2 * M_PI );
    double v = acos( y ) / M_PI;
    return Point2( u, v );
}

Sphere::Sphere(Point const &pos, double radius, Rotation const &rotation)
:
    position(pos),
    r(radius),
    rotation(rotation)
{}
