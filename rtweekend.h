#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <chrono>
#include <random>
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

inline std::mt19937 generate() {
    std::random_device rd{};

    std::seed_seq ss{
        static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()), rd(), rd(), rd(), rd(), rd(), rd(), rd()
    };

    return std::mt19937{ss};
}

inline std::mt19937 mt{ generate() };

inline double random_double() {
    return std::uniform_real_distribution{0.0, 1.0}(mt);
}

#include "vec3.h"
#include "color.h"
#include "interval.h"
#include "ray.h"

#endif //RTWEEKEND_H
