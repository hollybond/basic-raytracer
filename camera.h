#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
public:
    double  aspect_ratio        = 1.0;
    int     image_width         = 100;
    int     samples_per_pixel   = 10;
    int     max_depth           = 10;

    void render(const hittable &world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j{0}; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i{0}; i < image_width; ++i) {
                color pixel_color(0,0,0);
                for (int sample{}; sample < samples_per_pixel; ++sample) {
                    const ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\nDone.";
    }

private:
    int     image_height{};
    double  pixel_samples_scale{}; // color scale factor for sum of random pixels
    point3  center;
    point3  pixel00_loc;
    vec3    pixel_delta_u;
    vec3    pixel_delta_v;

    void initialize() {
        // image height must be at least 1
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = {0, 0, 0};

        constexpr double focal_length{1.0};
        constexpr double viewport_height{2.0};
        const double viewport_width{viewport_height * image_width / image_height};


        // vectors defining viewport horizontal and vertical edges
        // y-axis of camera is inverted to viewport
        const vec3 viewport_u{viewport_width, 0, 0};
        const vec3 viewport_v{0, -viewport_height, 0};

        // horizontal and vertical delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // find upper left pixel of viewport
        const vec3 viewport_upper_left{center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2};
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + 0.5 * pixel_delta_v);
    }

    ray get_ray(const int i, const int j) const {
        // ray from origin to randomly sampled point about i, j
        const auto offset = sample_square();
        const auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

        const auto ray_origin = center;
        const auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    vec3 sample_square() const {
        return { random_double() - 0.5, random_double() - 0.5, 0 };
    }

    static color ray_color(const ray &r, const int depth, const hittable &world) {
        if (depth <= 0)
            return {0,0,0};

        if (hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
            const vec3 direction = rec.normal + random_unit_vector();
            return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0};
    }
};
#endif //CAMERA_H
