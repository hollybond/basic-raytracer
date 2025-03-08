#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter (
        const ray& r, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    explicit lambertian(const color& albedo) : albedo(albedo) {};

    bool scatter(
        const ray &r, const hit_record &rec, color &attenuation, ray &scattered)
    const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // catch case where scatter direction is zero
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    explicit metal(const color& albedo, const double fuzz) : albedo(albedo), fuzz(fuzz) {};

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        // rays that are scattered beneath surface are absorbed
        return (dot(scattered.direction(), rec.normal) > 0);
    }
private:
    color albedo;
    double fuzz;
};

#endif //MATERIAL_H
