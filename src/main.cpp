#include "commons.h"

#include "vec3.h"
#include "Color.h"
#include "ray.h"

#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

#include "camera.h"

int main(void)
{
    // World

    HittableList world;
    
#pragma region r1
    // auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<Dielectric>(1.20);
    // auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    // auto material_right  = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.6);

    // world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));
#pragma endregion r1
#pragma region r2
    // auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    // world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    // for (i32 a = -11; a < 11; a++) {
    //     for (i32 b = -11; b < 11; b++) {
    //         auto choose_mat = random_double();
    //         Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

    //         if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
    //             shared_ptr<Material> sphere_material;

    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = Color::random() * Color::random();
    //                 sphere_material = make_shared<Lambertian>(albedo);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = Color::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = make_shared<Metal>(albedo, fuzz);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 sphere_material = make_shared<Dielectric>(1.3);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             }
    //         }
    //     }
    // }

    // auto material1 = make_shared<Dielectric>(1.7);
    // world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    // auto material2 = make_shared<Lambertian>(Color(0.6, 0.2, 0.3));
    // world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    // auto material3 = make_shared<Metal>(Color(0.2, 0.6, 0.5), 0.7);
    // world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
#pragma endregion r2
#pragma region r3
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    auto d1 = make_shared<Dielectric>(0.3);
    world.add(make_shared<Sphere>(Point3(0, 1, 1), 1.0, d1));
    auto d2 = make_shared<Dielectric>(1.7);
    world.add(make_shared<Sphere>(Point3(0, 1, 3), 1.0, d2));

    auto l1 = make_shared<Lambertian>(Color(0.6, 0.2, 0.3));
    world.add(make_shared<Sphere>(Point3(-4, 1, -1), 1.0, l1));

    auto m1 = make_shared<Metal>(Color(0.5, 0.3, 0.9), 0.7);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, m1));
    auto m2 = make_shared<Metal>(Color(0.6, 0.6, 0.3), 0.1);
    world.add(make_shared<Sphere>(Point3(4, 1, -2), 1.0, m2));

#pragma endregion

    // Camera

    Camera camera;
    camera.m_aspect_ratio      = 16.0 / 9.0;

    camera.m_image_width       = 400;
    camera.m_samples_per_pixel = 100;
    camera.m_max_depth         = 50;

    // camera.m_image_width       =  1280;
    // camera.m_samples_per_pixel =  250;
    // camera.m_max_depth         =  100;
 
    // camera.m_image_width       = 1920;
    // camera.m_samples_per_pixel = 500;
    // camera.m_max_depth         = 200;

    camera.m_gamma             = 2.0;
    camera.m_vfov              = 20;
    camera.m_look_from         = Point3(13, 2, 3);
    camera.m_look_at           = Point3(0, 0, 0);
    camera.m_vup               = Vec3(0, 1, 0);
    camera.m_focus_dist        = 10.0;
    camera.m_defocus_angle     = 0.5;

    camera.render(world);

    return 0;
}