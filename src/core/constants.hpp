#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Dependencies
#include <iostream>
#include <stdio.h> 
#include <GL/glew.h>
#include <GL/glut.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>
#include <memory>
#include <cstring>
 
namespace display {
inline int width = 1280;
inline int height = 720;
inline int _triangleCountCircle = 20;
inline int fps = 60;
}

namespace math {
constexpr float pi = 3.142857; 
constexpr float tau = 6.283185f;
constexpr float epsilon = 0.0001f;
}
#define DEG2RAD(x) (x * (math::pi/180))

// Type aliases
#define ObjectList std::vector<std::unique_ptr<Object>>

namespace physics {
inline float gravity = 0.5f;
inline float acceleration = 1.5f;
inline float friction = 1.f;
}

namespace color {
constexpr glm::vec3 white = glm::vec3(1.f, 1.f, 1.f);
constexpr glm::vec3 black = glm::vec3(0.f, 0.f, 0.f);
constexpr glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
constexpr glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
constexpr glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);
constexpr glm::vec3 yellow = glm::vec3(1.f, 1.f, 0.f);
constexpr glm::vec3 cyan = glm::vec3(0.f, 1.f, 1.f);
constexpr glm::vec3 magenta = glm::vec3(1.f, 0.f, 1.f);
constexpr glm::vec3 orange = glm::vec3(1.f, 0.5f, 0.f);
constexpr glm::vec3 purple = glm::vec3(0.5f, 0.f, 1.f);
constexpr glm::vec3 pink = glm::vec3(1.f, 0.5f, 0.5f);
constexpr glm::vec3 brown = glm::vec3(0.5f, 0.25f, 0.f);
constexpr bool use = true;
constexpr bool noUse = false;
}
#endif // CONSTANTS_HPP
