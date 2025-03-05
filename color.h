#ifndef COLOR_H
#define COLOR_H

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    const auto r = pixel_color.x();
    const auto g = pixel_color.y();
    const auto b = pixel_color.z();

    // translate components into byte range [0,255]
    const int rbyte = static_cast<int>(255.999 * r);
    const int gbyte = static_cast<int>(255.999 * g);
    const int bbyte = static_cast<int>(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif //COLOR_H
