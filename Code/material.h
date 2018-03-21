#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "triple.h"
#include "image.h"

#include <memory>

class Material
{
    public:
        // If 'hasTexture' is set, then 'pTexture' is set. Otherwise 'color' is set
        bool hasTexture;
        Color color;        // base color
        std::shared_ptr< Image > pTexture;
        double ka;          // ambient intensity
        double kd;          // diffuse intensity
        double ks;          // specular intensity
        double n;           // exponent for specular highlight size
        bool isFlat;        // True if no lighting can be applied

        Material() = default;

        Material(Color const &color, double ka, double kd, double ks, double n, bool isFlat)
        :
            hasTexture(false),
            color(color),
            ka(ka),
            kd(kd),
            ks(ks),
            n(n),
            isFlat(isFlat)
        {}

        Material(std::shared_ptr< Image > pTexture, double ka, double kd, double ks, double n, bool isFlat)
        :
            hasTexture(true),
            pTexture(pTexture),
            ka(ka),
            kd(kd),
            ks(ks),
            n(n),
            isFlat(isFlat)
        {}
};

#endif
