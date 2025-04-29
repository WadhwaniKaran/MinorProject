#include "camera.h"
#include <cmath>
#include <thread>
#include <vector>

#include "fmt.h"

void Camera::render(const HittableList &world)
{
    initialize();

    u32 total_threads = 4;
    u32 load_scanlines = std::ceil(m_image_height / total_threads);
    Color *buffer = (Color*)malloc(sizeof(Color) * m_image_height * m_image_width);

    fmt::hide_console_cursor();

    std::thread thread0(&Camera::threaded_render, this, world, buffer, 0, load_scanlines);
    std::thread thread1(&Camera::threaded_render, this, world, buffer, 1, load_scanlines);
    std::thread thread2(&Camera::threaded_render, this, world, buffer, 2, load_scanlines);
    std::thread thread3(&Camera::threaded_render, this, world, buffer, 3, load_scanlines);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();

    std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";
    for(u32 j = 0; j < m_image_height; j++)
    {
        for(u32 i = 0; i < m_image_width; i++)
        {
            color_write(std::cout, buffer[j * m_image_width + i], m_gamma);
        }
    }

    fmt::show_console_cursor();

    free(buffer);
}

void Camera::threaded_render(const HittableList &world, Color *buffer, u32 thread, u32 load_scanlines)
{
    FMT *logger = new FMT;

    size_t offset = m_image_width * load_scanlines * thread;

    for (u32 j = 0; j < load_scanlines; j++)
    {
        //std::clog << "\rThread " << thread << " " << load_scanlines - j << std::endl;
        for(u32 i = 0; i < m_image_width; i++)
        {
            Color pixel_color(0, 0, 0);
            for(int sample = 0; sample < m_samples_per_pixel; sample++)
            {
                Ray ray = get_ray(i, j + load_scanlines * thread);
                pixel_color += ray_color(ray, world, m_max_depth);
            }
            (buffer + offset)[j * m_image_width + i] = m_pixel_samples_scale * pixel_color;
            logger->progress_bar((f32)(j * m_image_width + i)/(load_scanlines * m_image_width));
        }
    }

    delete logger;
}

void Camera::initialize()
{
    m_center = m_look_from;

    m_image_height = u32(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    f64 theta = degrees_to_radians(m_vfov);
    f64 h = std::tan(theta / 2);
    f64 viewport_height = 2 * h * m_focus_dist;
    f64 viewport_width = viewport_height * ((f64)m_image_width/m_image_height);

    // camera basis vectors
    m_bw = unit_vector(m_look_from - m_look_at);
    m_bu = unit_vector(cross(m_vup, m_bw));
    m_bv = cross(m_bw, m_bu);

    // viewport vectors

    Vec3 viewport_u = viewport_width * m_bu;
    Vec3 viewport_v = viewport_height * -m_bv;

    // pixel delta vectors

    m_pixel_delta_u = viewport_u / m_image_width;
    m_pixel_delta_v = viewport_v / m_image_height;

    Vec3 viewport_upper_left = m_center - m_focus_dist * m_bw 
                            - 0.5 * (viewport_u + viewport_v);
    m_pixel_00_loc = viewport_upper_left + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    m_pixel_samples_scale = (m_samples_per_pixel == 0) ? 0 : 1.0 / m_samples_per_pixel;

    auto defocus_radius  = m_focus_dist * std::tan(degrees_to_radians(m_defocus_angle / 2));
    m_defocus_disk_u = m_bu * defocus_radius;
    m_defocus_disk_v = m_bv * defocus_radius;
}

Color Camera::ray_color(const Ray &ray, const HittableList &world, u32 depth)
{
    if(depth <= 0) return Color(0, 0, 0);

    HitRecord record;
    if(world.hit(ray, Interval(0.001, infinity), record)) // 0.001 -> to get rid of shadow acne problem
    {
        Ray scattered_ray;
        Color attenuation;
        if(record.material.get()->scatter(ray, record, attenuation, scattered_ray))
        {
            return attenuation * ray_color(scattered_ray, world, depth - 1);
        }
        return Color(0,0,0);
    }

    Vec3 unit_direction = unit_vector(ray.direction());
    f64 a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

Ray Camera::get_ray(u32 i, u32 j) const
{
    auto offset = sample_square();
    auto pixel_sample = m_pixel_00_loc 
                        + ((i + offset.x()) * m_pixel_delta_u)
                        + ((j + offset.y()) * m_pixel_delta_v);
    
    auto ray_origin = (m_defocus_angle <= 0) ? m_center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

Vec3 Camera::sample_square() const
{
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

Point3 Camera::defocus_disk_sample() const
{
    auto p = random_on_unit_disk();
    return m_center + (p[0] * m_defocus_disk_u) + (p[1] * m_defocus_disk_v);
}
