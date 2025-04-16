#ifndef CUSTOM_OBJECTS_HPP
#define CUSTOM_OBJECTS_HPP
#include "core/core.hpp"

class MyCircle : public Circle, public WithGravity {
public:
    MyCircle(int x, int y, float radius) :
        Circle(x, y, radius) {}
    MyCircle(Vector position, float radius) :
        Circle(position, radius) {}

    void update() override {
        updateGravity();
        position += velocity;
    }

    std::string toString() const override {
        return "MyCircle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }
};


#endif // CUSTOM_OBJECTS_HPP