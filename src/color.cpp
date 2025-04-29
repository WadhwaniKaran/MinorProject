#include "color.h"

// gamma - 2
inline f64 linear_to_gamma(f64 linear_component, f64 gamma)
{
    if(linear_component > 0)
        return std::pow(linear_component, 1 / gamma);
    else return 0.0;
}

void color_write(std::ostream &out, const Color &pixel_color, f64 gamma)
{
    f64 r = pixel_color.x();
    f64 g = pixel_color.y();
    f64 b = pixel_color.z();

    r = linear_to_gamma(r, gamma);
    g = linear_to_gamma(g, gamma);
    b = linear_to_gamma(b, gamma);

    static const Interval intensity(0.0, 1.0);

    i32 rbyte = i32(255.999 * intensity.clamp(r));
    i32 gbyte = i32(255.999 * intensity.clamp(g));
    i32 bbyte = i32(255.999 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}