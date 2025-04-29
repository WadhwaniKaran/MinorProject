#pragma once

#include "commons.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "material.h"

class Camera
{
    public:
    f64 m_aspect_ratio      = 1.0;
    u32 m_image_width       = 100;
    u32 m_samples_per_pixel = 10;
    u32 m_max_depth         = 10; // max depth for ray bouncing
    f64 m_gamma             = 2.0;
    f64 m_vfov              = 90;
    Point3 m_look_from      = Point3(0, 0, 0);
    Point3 m_look_at        = Point3(0, 0, -1);
    Vec3 m_vup              = Vec3(0, 0, 0);
    f64 m_defocus_angle     = 0;
    f64 m_focus_dist        = 10;


    void render(const HittableList &world);

    private:
    u32    m_image_height;
    Point3 m_center;
    Point3 m_pixel_00_loc;
    Vec3   m_pixel_delta_u;
    Vec3   m_pixel_delta_v;
    f64    m_pixel_samples_scale;
    Vec3   m_bu, m_bv, m_bw; // camera basis vectors
    Vec3   m_defocus_disk_u, m_defocus_disk_v;

    void initialize();
    void threaded_render(const HittableList &world, Color *buffer, u32 thread, u32 load_scanlines);

    Color ray_color(const Ray &ray, const HittableList &world, u32 depth);

    Ray get_ray(u32 i, u32 j) const;

    Vec3 sample_square() const;
    Point3 defocus_disk_sample() const;
};