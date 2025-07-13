#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "constants.hpp"

#define vec00 (vec2(0.f, 0.f))
#define vec001 (vec3(0.f, 0.f, 1.f))
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

inline vec3 vec2ToVec3(vec2 v, float z=0.f) {
    return vec3(v.x, v.y, z);
}

inline vec4 vec2ToVec4(vec2 v, float z=0.f, float w=1.f) {
    return vec4(v.x, v.y, z, w);
}

inline vec2 vec4ToVec2(vec4 v) {
    return vec2(v.x, v.y);
}

#endif // VECTOR_HPP
