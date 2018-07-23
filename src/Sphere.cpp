//
// Created by 長井亨 on 2018/07/18.
//
#include "Sphere.hpp"

namespace nagato {

    std::optional <Hit> Sphere::intersect(Ray &ray, float tmin, float tmax) {
      const Vector3 op = position - ray.origin;
      const float b = dot(op, ray.direction);
      const float det = b * b - dot(op, op) + radius * radius;

      if (det < 0) {
        return {};
      }
      const float t1 = static_cast<const float>(b - sqrt(det));

      if (tmin < t1 && t1 < tmax) {
        auto point = ray.origin + ray.direction * t1;
        auto normal = (point - position) / radius;
        return Hit{t1, point, normal, this};
      }

      const float t2 = static_cast<const float>(b + sqrt(det));
      if (tmin < t2 && t2 < tmax) {
        auto point = ray.origin + ray.direction * t2;
        auto normal = (point - position) / radius;
        return Hit{t2, point, normal, this};
      }
      return {};
    }

    Sphere::Sphere(Vector3 p, float r, SurfaceType t, Spectrum color, Spectrum em) : Object(p, t, color, em), radius(r)
    {

    }
}