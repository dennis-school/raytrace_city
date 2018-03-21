#ifndef SCENE_H_
#define SCENE_H_

#include "light.h"
#include "object.h"
#include "triple.h"
#include "hit.h"
#include "ray.h"

#include <vector>

// Forward declerations
class Ray;
class Image;

class Scene
{
    std::vector<ObjectPtr> objects;
    std::vector<LightPtr> lights;   // no ptr needed, but kept for consistency

    public:
        Scene( ): hasAmbientLight( false ) { }

        // trace a ray into the scene and return the color
        Color trace(Ray const &ray);

        // render the scene to the given image
        void render(Image &img);


        void addObject(ObjectPtr obj);
        void addLight(Light const &light);
        void setEye(Triple const &position);
        void setEyePitch(float eyePitch);
        void setHasShadows(bool hasShadows);
        void setMaxRecursionDepth( unsigned int maxRecursionDepth );
        void setSuperSamplingFactor( unsigned int factor );
        void setAmbientLight(Color const &color );

        unsigned getNumObject();
        unsigned getNumLights();

    private:
        Point eye;
        float eyePitch;
        bool hasShadows;
        // True if a fixed ambient light is set
        // otherwise take average of lights
        bool hasAmbientLight;
        Color ambientLight;
        unsigned int maxRecursionDepth;
        unsigned int superSamplingFactor;

        bool hit(Ray const &ray, Hit& dstHit, ObjectPtr& dstObj);
        Color trace(Ray const &ray, int recDepth);
};

#endif
