//
// Created by 長井亨 on 2018/09/06.
//

#ifndef PATHTRACING_FRESNEL_HPP
#define PATHTRACING_FRESNEL_HPP

#include "BSDF.hpp"

namespace nagato {
    class Fresnel : public BSDF {
    public:
        Fresnel(Spectrum c);

        Spectrum makeNewDirection(
                int *wavelengthIndex,
                Vector3 *newDirection,
                Ray &ray,
                const Hit &surfaceInfo) const override;
    };
}

#endif //PATHTRACING_FRESNEL_HPP
