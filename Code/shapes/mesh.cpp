/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "mesh.h"
#include "../objloader.h"

#include <cmath>
#include <iostream>

using namespace std;

AABB::AABB( ) {

}

AABB::AABB( Point const &lowBound, Point const &uppBound )
    : lowBound( lowBound ), uppBound( uppBound ) {
}

bool AABB::intersects( Ray const &ray ) {
  double invDirX = 1.0f / ray.D.x;
  double invDirY = 1.0f / ray.D.y;
  double invDirZ = 1.0f / ray.D.z;

  // "Clip" the line within the box, along each axis
  double tX1 = ( lowBound.x - ray.O.x ) * invDirX;
  double tX2 = ( uppBound.x - ray.O.x ) * invDirX;
  double tY1 = ( lowBound.y - ray.O.y ) * invDirY;
  double tY2 = ( uppBound.y - ray.O.y ) * invDirY;
  double tZ1 = ( lowBound.z - ray.O.z ) * invDirZ;
  double tZ2 = ( uppBound.z - ray.O.z ) * invDirZ;

  double tXMin = min(tX1, tX2);
  double tYMin = min(tY1, tY2);
  double tZMin = min(tZ1, tZ2);
  double tXMax = max(tX1, tX2);
  double tYMax = max(tY1, tY2);
  double tZMax = max(tZ1, tZ2);

  double tMin = max(max(tXMin, tYMin), tZMin);
  double tMax = min(min(tXMax, tYMax), tZMax);

  // Test: Not behind ray origin and it does intersect
  return tMax > 0 && tMin < tMax;
}

/** Extracts the location from the vertex */
Point toPoint( Vertex v ) {
      return Point( v.x, v.y, v.z );
}

Hit Mesh::intersect( const Ray& ray ) {
    if ( triangles.size( ) == 0 || !aabb.intersects( ray ) )
        return Hit::NO_HIT( );

    // Look for the triangle closest to the camera that intersects with the ray.
    Hit h = triangles[ 0 ].intersect( ray );
    for ( unsigned int i = 1; i < triangles.size( ); i++ ) {
        Hit newH = triangles[ i ].intersect( ray );
        
        if ( isnan( h.t ) || h.t <= 0 || ( newH.t < h.t && newH.t > 0 ) ) {
            h = newH;
        }
    }

    if ( h.t <= 0 )
        return Hit::NO_HIT( );

    return h;
}

Mesh::Mesh( Point const &position, double scale, const std::string& filepath ) {
    OBJLoader objLoader( filepath );
    std::vector<Vertex> vertexData = objLoader.vertex_data( );

    double minX = std::numeric_limits<double>::infinity( );
    double minY = std::numeric_limits<double>::infinity( );
    double minZ = std::numeric_limits<double>::infinity( );
    double maxX = -std::numeric_limits<double>::infinity( );
    double maxY = -std::numeric_limits<double>::infinity( );
    double maxZ = -std::numeric_limits<double>::infinity( );

    // Note that the position and scale are currently applied directly to the vertices
    // Keep it like this for now, as it is faster to do it here. Though it is less flexible.
    for ( unsigned int i = 0; i < vertexData.size( ); i += 3 ) {
        Point p1 = position + toPoint( vertexData[ i ] ) * scale;
        Point p2 = position + toPoint( vertexData[ i + 1 ] ) * scale;
        Point p3 = position + toPoint( vertexData[ i + 2 ] ) * scale;
        Triangle t( p1, p2, p3 );
        triangles.push_back( t );

        minX = min( min( minX, p1.x ), min( p2.x, p3.x ) );
        minY = min( min( minY, p1.y ), min( p2.y, p3.y ) );
        minZ = min( min( minZ, p1.z ), min( p2.z, p3.z ) );
        maxX = max( max( maxX, p1.x ), max( p2.x, p3.x ) );
        maxY = max( max( maxY, p1.y ), max( p2.y, p3.y ) );
        maxZ = max( max( maxZ, p1.z ), max( p2.z, p3.z ) );
    }

    aabb = AABB( Point( minX, minY, minZ ), Point( maxX, maxY, maxZ ) );
}
