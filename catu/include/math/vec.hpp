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

   vec3 normalized() const;

   vec3 operator-(const vec3 other) {
    auto p = *this;
    p.x-=other.x;
    p.y-=other.y;
    p.z-=other.z;
    return p;
   }
    vec3 operator+(const vec3 other){
        auto p = *this;
        p.x+=other.x;
        p.y+=other.y;
        p.z+=other.z;
        return p;
   }
};

template<typename T>
T inter(T from, T to, float t) {
    return from*(1-t) + to * t;
}
vec3 Interpolate(vec3 from, vec3 to, float t);

vec3 Cross(vec3 one, vec3 two);


