#pragma once

#include "commons.h"
#include "hittable.h"
#include "color.h"


class Material
{
    public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray_in, 
        const HitRecord &record, Color &attenuation, Ray &scattered) const { return false; }
};

class Lambertian : public Material
{
    public:
    Lambertian(const Color &albedo) : m_albedo(albedo) {}

    bool scatter(const Ray &ray_in, 
        const HitRecord &record, Color &attenuation, Ray &scattered) const override
    {
        Vec3 scatter_direction = record.normal + random_unit_vector();
        
        if(scatter_direction.near_zero()) scatter_direction = record.normal;

        scattered = Ray(record.point, scatter_direction);
        attenuation = m_albedo;
        return true;
    }

    private:
    Color m_albedo;
};

class Metal : public Material
{
    public:
    Metal(const Color &albedo, f64 fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray &ray_in, 
        const HitRecord &record, Color &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = reflect(ray_in.direction(), record.normal);
        reflected = unit_vector(reflected) + (m_fuzz * random_unit_vector());

        scattered = Ray(record.point, reflected);
        attenuation = m_albedo;
        return (dot(scattered.direction(), record.normal) > 0);
    }

    private:
    Color m_albedo;
    f64   m_fuzz;
};

class Dielectric : public Material
{
    public:
    Dielectric(f64 refractive_index) : m_refractive_index(refractive_index) {}

    bool scatter(const Ray &ray_in, 
        const HitRecord &record, Color &attenuation, Ray &scattered) const override
    {
        attenuation = Color(1,1,1);
        f64 ri = (record.front_face) ? (1/m_refractive_index) : m_refractive_index;

        Vec3 unit_direction = unit_vector(ray_in.direction());

        f64 cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
        f64 sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = (ri * sin_theta) > 1.0;
        Vec3 direction;

        if(cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, record.normal);
        else 
            direction = refract(unit_direction, record.normal, ri);


        scattered = Ray(record.point, direction);
        return true;
    }

    private:
    f64 m_refractive_index;

    // Schlick Approximation
    static f64 reflectance(f64 cos_theta, f64 refractive_index)
    {
        auto r0 = (1 - refractive_index) / (1 + refractive_index);
        r0 = r0*r0;
        return r0 + (1-r0) * std::pow((1 - cos_theta), 5);
    }
};