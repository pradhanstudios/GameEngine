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
#define TOP_COLLISION 0b1000
#define BOTTOM_COLLISION 0b0100
#define LEFT_COLLISION 0b0010
#define RIGHT_COLLISION 0b0001
#define epsilon 0.0001f
#endif // CONSTANTS_HPP