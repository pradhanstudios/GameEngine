#ifndef CUSTOM_OBJECTS_HPP
#define CUSTOM_OBJECTS_HPP
#include "core/core.hpp"

class MyCircle : public Circle, public WithGravity, public Collision {
public:
    MyCircle(int x, int y, float radius, Texture* texture=nullptr) :
        Circle(x, y, radius, texture) {}
    MyCircle(Vector position, float radius, Texture* texture=nullptr) :
        Circle(position, radius, texture) {}

    void draw() override {
        drawCircleManual(position, texture, radius, DEG2RAD(90.f), imageShader);
    }

    void update() override {
        updateGravity();
        applyCollisions(velocity, acceleration);
        position += velocity;
    }

    std::string toString() const override {
        return "MyCircle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }
};

class MyRectangle : public Rectangle {
public:
    Vector3 color;
    
    MyRectangle(int x, int y, int width, int height, Vector3 color, Texture* texture=nullptr) : Rectangle(x, y, width, height, texture), color(color) {}
    MyRectangle(Vector position, int width, int height, Vector3 color, Texture* texture=nullptr) : Rectangle(position, width, height, texture), color(color) {}

    void draw() override {
        drawRectangleManual(position, texture, width, height, imageShader, color, USE_COLOR);
    }

};

#endif // CUSTOM_OBJECTS_HPP
