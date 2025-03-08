cmake_minimum_required(VERSION 3.30)
project(raytracing)

set(CMAKE_CXX_STANDARD 23)

add_executable(raytracing main.cpp
        rtweekend.h
        color.h
        vec3.h
        interval.h
        ray.h
        hittable.h
        sphere.h
        hittable_list.h
        camera.h
        material.h
)
