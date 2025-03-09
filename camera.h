#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
    double  aspect_ratio        = 1.0;
    int     image_width         = 100;
    int     samples_per_pixel   = 10;
    int     max_depth           = 10;

    double vfov     = 90;
    point3 lookfrom = point3(0,0,0);
    point3 lookat   = point3(0,0,-1);
    point3 vup      = point3(0,1,0);

    double defocus_angle    = 0;
    double focus_dist       = 10;

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
    vec3    u, v, w;
    vec3    defocus_disk_u;
    vec3    defocus_disk_v;

    void initialize() {
        // image height must be at least 1
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        const double theta = degrees_to_radians(vfov);
        const double h = std::tan(theta/2);
        const double viewport_height = 2 * h * focus_dist;
        const double viewport_width{viewport_height * image_width / image_height};

        // u, v, w basis of camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // vectors defining viewport horizontal and vertical edges
        // y-axis of camera is inverted to viewport
        const vec3 viewport_u = viewport_width * u;
        const vec3 viewport_v = viewport_height * -v;

        // horizontal and vertical delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // find upper left pixel of viewport
        const vec3 viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + 0.5 * pixel_delta_v);

        //calculate camera defocus disk basis vectors
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(const int i, const int j) const {
        // construct camera ray from defocus disk to randomly sampled point around
        // pixel location i, j
        const auto offset = sample_square();
        const auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

        const auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        const auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    vec3 sample_square() const {
        return { random_double() - 0.5, random_double() - 0.5, 0 };
    }

    point3 defocus_disk_sample() const {
        // returns random point on defocus disk
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    static color ray_color(const ray &r, const int depth, const hittable &world) {
        if (depth <= 0)
            return {0,0,0};

        if (hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return {0,0,0};
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0};
    }
};
#endif //CAMERA_H
