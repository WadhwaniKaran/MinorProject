#pragma once

#include "commons.h"
#include "vec3.h"
#include "interval.h"

typedef Vec3 Color;

void color_write(std::ostream& out, const Color &pixel_color, f64 gamma);