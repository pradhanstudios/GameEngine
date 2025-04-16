#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <iostream>
#include <stdio.h> 
#include <GL/glut.h> 
#include <math.h>
#include <vector>
#include <memory>
#define RESOLUTION_WIDTH 1280
#define RESOLUTION_HEIGHT 720
#define pi 3.142857 
#define tau 6.283185
#define TRIANGLE_COUNT_CIRCLE 20
#define ObjectList std::vector<std::unique_ptr<Object>>
inline float gravity = 0.5f;
inline float acceleration = 1.f;
inline float friction = -0.12f;
inline int fps = 60;
#endif // CONSTANTS_HPP