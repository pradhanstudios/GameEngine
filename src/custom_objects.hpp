#ifndef CUSTOM_OBJECTS_HPP
#define CUSTOM_OBJECTS_HPP
#include "core/core.hpp"

class MyCircle : public Circle, public WithGravity, public Collision {
public:
    MyCircle(float x, float y, float radius, Texture* texture=nullptr) :
        Circle(x, y, radius, 0.f, texture) {}
    MyCircle(vec2 position, float radius, Texture* texture=nullptr) :
        Circle(position, radius, 0.f, texture) {}

    void draw() override {
        drawCircle(position, radius, DEG2RAD(90.f), texture, Shader::Shape, Color::White, Color::NoUse);
    }

    void update() override {
        applyGravity();
        applyCollisions(position, velocity, acceleration);
        velocity += acceleration * Physics::Acceleration;
        position += velocity;
    }

    std::string toString() const override {
        return "MyCircle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }
};

class CollisionRectangle : public Rectangle, public WithGravity, public Collision {
public:
    CollisionRectangle(float x, float y, float width, float height, Texture* texture=nullptr, float rotation=0.f) : Rectangle(x, y, width, height, rotation, texture) {}
    void update() override {
        applyGravity();
        applyCollisions(position, velocity, acceleration);
        velocity += acceleration * Physics::Acceleration;
        position += velocity;
    }
};

class MyRectangle : public Rectangle {
public:
    vec3 color;
    
    MyRectangle(float x, float y, float width, float height, vec3 color, Texture* texture=nullptr) : Rectangle(x, y, width, height, DEG2RAD(10.f), texture), color(color) {}
    MyRectangle(vec2 position, float width, float height, vec3 color, Texture* texture=nullptr) : Rectangle(position, width, height, DEG2RAD(10.f), texture), color(color) {}

    void draw() override {
        drawRectangle(getCenter(), width, height, rotation, texture, Shader::Shape, color, Color::Use);
    }
};

#endif // CUSTOM_OBJECTS_HPP
