#ifndef COLOR_H
#define COLOR_H

#include "interval.h"

using color = vec3;

inline void write_color(std::ostream& out, const color& pixel_color) {
    const auto r = pixel_color.x();
    const auto g = pixel_color.y();
    const auto b = pixel_color.z();

    // translate components into byte range [0,255]
    static const interval intensity(0.000, 0.999);
    const int rbyte = static_cast<int>(256 * intensity.clamp(r));
    const int gbyte = static_cast<int>(256 * intensity.clamp(g));
    const int bbyte = static_cast<int>(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif //COLOR_H
