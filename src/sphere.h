#pragma once

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
    public:
    Sphere(const Point3 &center, f64 radius, shared_ptr<Material> material) 
        : m_center(center), m_radius(std::fmax(0, radius)), m_material(material) {}

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override
    {
        Vec3 oc = m_center - ray.origin();
        f64 a = ray.direction().length_squared();
        f64 h = dot(ray.direction(), oc);
        f64 c = oc.length_squared() - m_radius * m_radius;
        f64 discriminant = h * h - a * c;

        if(discriminant < 0) { return false; }

        f64 root_dis = std::sqrt(discriminant);

        f64 root = (h - root_dis) / a;
        if(!ray_t.surrounds(root))
        {
            root = (h + root_dis) / a;
            if(!ray_t.surrounds(root)) { return false; }
        }

        record.t = root;
        record.point = ray.at(root);
        Vec3 outward_normal = (record.point - m_center) / m_radius; // unit vector
        record.set_face_normal(ray, outward_normal);
        record.material = m_material;

        return true;
    }

    private:
    Point3               m_center;
    f64                  m_radius;
    shared_ptr<Material> m_material;
};
