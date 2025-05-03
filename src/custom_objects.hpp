#ifndef CUSTOM_OBJECTS_HPP
#define CUSTOM_OBJECTS_HPP
#include "core/core.hpp"

class MyCircle : public Circle, public WithGravity, public Collision {
public:
    MyCircle(int x, int y, float radius, Texture* texture) :
        Circle(x, y, radius, texture) {}
    MyCircle(Vector position, float radius, Texture* texture) :
        Circle(position, radius, texture) {}

    void update() override {
        updateGravity();
        applyCollisions(velocity, acceleration);
        position += velocity;
    }

    std::string toString() const override {
        return "MyCircle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }
};


#endif // CUSTOM_OBJECTS_HPP
