#include <math/vec.hpp>


vec3 Interpolate(vec3 from, vec3 to, float t) {
    return vec3{inter(from.x, to.x, t),inter(from.y, to.y, t),inter(from.z, to.z, t)};
}

vec3 Cross(vec3 l, vec3 r){
    vec3 ret;
    ret.x = l.y * r.z - l.z*r.y;
    ret.y = l.z * r.x - l.x*r.z;
    ret.z = l.x * r.y - l.y*r.x;
    return ret;
}

vec3 vec3::normalized() const {
    vec3 ret = *this;
    auto l = len();
    ret.x/=l;
    ret.y/=l;
    ret.z/=l;
    return ret;
}
