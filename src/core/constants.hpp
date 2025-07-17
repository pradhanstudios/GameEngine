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
 
namespace Display {
inline int Width = 1280;
inline int Height = 720;
inline int _TriangleCountCircle = 20;
inline int Fps = 60;
}

namespace Math {
constexpr float Pi = 3.142857; 
constexpr float Tau = 6.283185f;
constexpr float Epsilon = 0.0001f;
}
#define DEG2RAD(x) (x * (Math::Pi/180))

// Type aliases
#define ObjectList std::vector<std::unique_ptr<Object>>

namespace Physics {
inline float Gravity = 0.5f;
inline float Acceleration = 1.5f;
inline float Friction = 1.f;
}

namespace Color {
constexpr glm::vec3 White = glm::vec3(1.f, 1.f, 1.f);
constexpr glm::vec3 Black = glm::vec3(0.f, 0.f, 0.f);
constexpr glm::vec3 Red = glm::vec3(1.f, 0.f, 0.f);
constexpr glm::vec3 Green = glm::vec3(0.f, 1.f, 0.f);
constexpr glm::vec3 Blue = glm::vec3(0.f, 0.f, 1.f);
constexpr glm::vec3 Yellow = glm::vec3(1.f, 1.f, 0.f);
constexpr glm::vec3 Cyan = glm::vec3(0.f, 1.f, 1.f);
constexpr glm::vec3 Magenta = glm::vec3(1.f, 0.f, 1.f);
constexpr glm::vec3 Orange = glm::vec3(1.f, 0.5f, 0.f);
constexpr glm::vec3 Purple = glm::vec3(0.5f, 0.f, 1.f);
constexpr glm::vec3 Pink = glm::vec3(1.f, 0.5f, 0.5f);
constexpr glm::vec3 Brown = glm::vec3(0.5f, 0.25f, 0.f);
constexpr bool Use = true;
constexpr bool NoUse = false;
}
#endif // CONSTANTS_HPP
