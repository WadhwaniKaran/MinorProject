#pragma once

#include "commons.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"

class Material;

struct HitRecord
{
    Point3               point;
    Vec3                 normal; // always points opposite to incident ray && unit vector
    f64                  t;
    bool                 front_face;
    shared_ptr<Material> material;

    // outward normal assumed to be of unit length
    void set_face_normal(const Ray &ray, const Vec3 &outward_normal)
    {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
    public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const = 0;
};