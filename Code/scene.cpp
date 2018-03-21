#include "scene.h"

#include "image.h"
#include "material.h"

#include <cmath>
#include <limits>

#include <iostream>

using namespace std;

// A constant value that ensures floating-point errors do not cause problems
// Mainly used for shadow and reflection rays
const float SHADOW_BIAS = 1e-4;

bool Scene::hit(Ray const &ray, Hit& dstHit, ObjectPtr& dstObj) {
    // Find hit object and distance
    Hit min_hit = Hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    if ( !obj )
        return false;

    dstHit = min_hit;
    dstObj = obj;

    return true;
}

Color Scene::trace(Ray const &ray) {
    return trace( ray, maxRecursionDepth );
}

Color Scene::trace(Ray const &ray, int recDepth)
{
    Hit min_hit = Hit::NO_HIT( );
    ObjectPtr obj = nullptr;
    // No hit? Return background color.
    if (!hit(ray, min_hit, obj)) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;             //the hit objects material
    Point hitPoint = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    // Phong color calculation

    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    for ( LightPtr pLight : lights ) {
        // Note that the ambient color is chosen as the average of all light sources
        // (as proposed by the lecture slides)
        ambientColor += ( pLight->color * material.ka ) / lights.size( );
        // Normalized vector pointing to the light
        Vector L = ( pLight->position - hitPoint ).normalized( );
        float lightDistance = ( pLight->position - hitPoint ).length( );

        Ray shadowRay( hitPoint + L * SHADOW_BIAS, L );
        Hit shadowHit = Hit::NO_HIT( );
        ObjectPtr shadowObj = nullptr;

        if ( !hasShadows || !hit( shadowRay, shadowHit, shadowObj ) || shadowHit.t > lightDistance ) {
          // Mirror of light vector along the surface normal
          Vector RLight = 2 * L.dot( N ) * N - L;
          diffuseColor += pLight->color * material.kd * max( 0.0, N.dot( L ) );
          specularColor += pLight->color * material.ks * pow( max( 0.0, RLight.dot( V ) ), material.n );
        }
    }

    if ( hasAmbientLight ) {
        ambientColor = this->ambientLight * material.ka;
    }

    if ( recDepth > 0 && material.ks > 0 ) {
        Vector REye = 2 * V.dot( N ) * N - V;
        Ray specularRay( hitPoint + REye * SHADOW_BIAS, REye );
        specularColor += material.ks * trace( specularRay, recDepth - 1 );
    }

    Color materialColor;
    if ( material.hasTexture ) {
        Point2 uv = obj->uvMap( hitPoint );
        materialColor = material.pTexture->colorAt( uv.x, uv.y );
    } else {
        materialColor = material.color;
    }
    Color color;
    if ( material.isFlat ) {
        color = materialColor;
    } else {
        // Note that the specular color is unrelated to the material color (as it is a reflection of the light source)
        color = ( ambientColor + diffuseColor ) * materialColor + specularColor;
    }

    return color;
}

void rotate( double &x, double &y, double angle ) {
    double c = cos( angle );
    double s = sin( angle );

    double nx = c * x - s * y;
    double ny = s * x + c * y;
    x = nx;
    y = ny;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();

    unsigned int ssFactor = std::max( superSamplingFactor, (unsigned int) 1 );

    #pragma omp parallel for
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            Color avgCol;
            for ( unsigned int ssY = 0; ssY < ssFactor; ssY++ ) {
                for ( unsigned int ssX = 0; ssX < ssFactor; ssX++ ) {
                    double ssXDisplacement = ( ssX + 1 ) / (double) ( ssFactor + 1 );
                    double ssYDisplacement = ( ssY + 1 ) / (double) ( ssFactor + 1 );

                    Point pixel(x + ssXDisplacement, h - 1 - y + ssYDisplacement, 0);
                    Vector rayDir = (pixel - eye).normalized( );
                    rotate(rayDir.y, rayDir.z, eyePitch);
                    Ray ray(eye, rayDir);
                    Color col = trace(ray);
                    col.clamp();
                    
                    avgCol += col;
                }
            }
            avgCol /= ssFactor * ssFactor;
            img(x, y) = avgCol;
        }
    }
}

// --- Misc functions ----------------------------------------------------------

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

void Scene::setEyePitch(float eyePitch)
{
    this->eyePitch = eyePitch;
}

void Scene::setHasShadows( bool hasShadows ) {
    this->hasShadows = hasShadows;
}

void Scene::setMaxRecursionDepth( unsigned int maxRecursionDepth ) {
    this->maxRecursionDepth = maxRecursionDepth;
}

void Scene::setSuperSamplingFactor( unsigned int factor ) {
    this->superSamplingFactor = factor;
}

void Scene::setAmbientLight(Color const &color ) {
    hasAmbientLight = true;
    this->ambientLight = color;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
