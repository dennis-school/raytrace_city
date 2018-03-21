/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef MESH_H_
#define MESH_H_

#include "../object.h"
#include "./triangle.h"

class AABB {
public:
  AABB( );
  AABB( Point const &lowBound, Point const &uppBound );
  bool intersects( Ray const &ray );
private:
  Point lowBound;
  Point uppBound;
};

/**
 * A Mesh is a collection of triangles that share the same material.
 */
class Mesh: public Object {
    public:
        Mesh( Point const &position, double scale, const std::string& filepath );

        virtual Hit intersect(Ray const &ray);

    private:
        AABB aabb;
        std::vector< Triangle > triangles;
};

#endif
