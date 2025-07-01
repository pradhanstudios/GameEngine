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
 
// Resolution / Quality
#define RESOLUTION_WIDTH 1280
#define RESOLUTION_HEIGHT 720
#define TRIANGLE_COUNT_CIRCLE 20
inline int fps = 60;

// Math
#define pi 3.142857 
#define tau 6.283185
#define epsilon 0.0001f
#define DEG2RAD(x) (x * (pi/180))

// Type aliases
#define ObjectList std::vector<std::unique_ptr<Object>>

// Collisions
#define TOP_COLLISION 0b1000
#define BOTTOM_COLLISION 0b0100
#define LEFT_COLLISION 0b0010
#define RIGHT_COLLISION 0b0001

// Physics
inline float gravity = 0.5f;
inline float acceleration = 1.f;
inline float friction = 1.f;

// Colors
#define WHITE vec3(1.f, 1.f, 1.f)
#define BLACK vec3(0.f, 0.f, 0.f)
#define RED vec3(1.f, 0.f, 0.f)
#define GREEN vec3(0.f, 1.f, 0.f)
#define BLUE vec3(0.f, 0.f, 1.f)
#define YELLOW vec3(1.f, 1.f, 0.f)
#define CYAN vec3(0.f, 1.f, 1.f)
#define MAGENTA vec3(1.f, 0.f, 1.f)
#define ORANGE vec3(1.f, 0.5f, 0.f)
#define PURPLE vec3(0.5f, 0.f, 1.f)
#define PINK vec3(1.f, 0.5f, 0.5f)
#define BROWN vec3(0.5f, 0.25f, 0.f)
#define USE_COLOR 1
#define NO_USE_COLOR 0
#endif // CONSTANTS_HPP
