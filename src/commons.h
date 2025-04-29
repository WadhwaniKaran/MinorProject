#pragma once

#include "typedefs.h"

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

const f64 infinity = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

inline f64 degrees_to_radians(f64 degrees)
{
    return degrees * pi / 180;
}

// between 0.0 and 1.0
inline f64 random_double()
{
    static std::uniform_real_distribution<f64> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline f64 random_double(f64 min, f64 max)
{
    return min + (max - min) * random_double();
}