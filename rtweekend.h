#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

inline double degrees_to_radians(const double degrees) {
    return degrees * pi / 180;
}

#include "vec3.h"
#include "color.h"
#include "interval.h"
#include "ray.h"

#endif //RTWEEKEND_H
