//
// Created by 長井亨 on 2018/09/06.
//

#include "Fresnel.hpp"

namespace nagato
{

    Fresnel::Fresnel(Material *m) : BSDF(m)
    {

    }

    Spectrum Fresnel::makeNewDirection(
            int *wavelengthIndex,
            Vector3 *newDirection,
            Ray &ray,
            const Hit &surfaceInfo) const
    {
        // #TODO : 波長ごとに異なる屈折率を扱えるようにする
        float ior = 1.5;

        const auto wi = -ray.direction;
        const auto into = dot(wi, surfaceInfo.normal) > 0;
        const auto n = into ? surfaceInfo.normal : -surfaceInfo.normal;
        const auto eta = into ? 1 / ior : ior;
        const auto wt = [&]() -> std::optional<Vector3> {
            const auto t = dot(wi, n);
            const auto t2 = 1 - eta * eta * (1 - t * t);
            if (t2 < 0) {
                return {};
            };
            return (n * t - wi) * eta - n * sqrt(t2);
        }();
        if (!wt) {
            *newDirection = surfaceInfo.normal * 2 * dot(wi, surfaceInfo.normal) - wi;
            return material->color;
        }
        const auto Fr = [&]() {
            const auto cos = into ? dot(wi, surfaceInfo.normal) : dot(*wt, surfaceInfo.normal);
            const auto r = (1 - ior) / (1 + ior);
            return r * r + (1 - r * r) * pow(1 - cos, 5);
        }();

        if (Random::Instance().next() < Fr) {
            *newDirection = surfaceInfo.normal * 2 * dot(wi, surfaceInfo.normal) - wi;
            return material->color * Fr;
        } else {
            *newDirection = *wt;
            return material->color * (1 - Fr);
        }
    }
}