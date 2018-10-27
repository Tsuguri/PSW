#pragma once
#include <cmath>

struct vec2{
    float u;
    float v;
};

struct vec3{
   float x;
   float z;
   float y;

   vec3(float x, float y, float z) : x(x), y(y), z(z){}
   vec3() : x(0), y(0), z(0){}

   float len() const {
        return std::sqrt(x*x + y*y + z*z);
   }
};

