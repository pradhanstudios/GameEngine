#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "constants.hpp"
#include "object.hpp"
#include "vector.hpp"

class Rectangle : public Object {
public:
    Rectangle(int x, int y, int width, int height) :
        Object(x, y, width, height) {}

    Rectangle(Vector position, int width, int height) :
        Object(position, width, height) {}

    bool isInside(Vector point) override {
        return (point.x >= position.x && point.x <= position.x + width &&
                point.y >= position.y && point.y <= position.y + height);
    }

    bool isColliding(Object& other) override;

    void draw() override {
        glRectf(position.x, position.y, position.x + width, position.y + height);
    }

    virtual void update() {
        ;
    }

    std::string toString() const {
        return "Rectangle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    void setCenter(Vector center) override {
        position.x = center.x - width / 2;
        position.y = center.y - height / 2;
    }

    void setCenterX(float centerX) override {
        position.x = centerX - width / 2;
    }

    void setCenterY(float centerY) override {
        position.y = centerY - height / 2;
    }
};

class Circle : public Object {
public:
    float radius;
    Circle(int x, int y, float radius) :
        Object(x, y, radius * 2, radius * 2), radius(radius) {}

    Circle(Vector position, float radius) :
        Object(position, radius * 2, radius * 2), radius(radius) {}

    bool isInside(Vector point) override {
        return position.distanceTo(point) <= radius;
    }

    bool isColliding(Object& other) override;

    void draw() override {
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(position.x, position.y);
            for (int i = 0; i <= TRIANGLE_COUNT_CIRCLE; i++) {
                float angle = i * tau / TRIANGLE_COUNT_CIRCLE;
                glVertex2f(position.x + radius * cos(angle), position.y + radius * sin(angle));
            }
        glEnd();
    }

    virtual void update() {
        ;
    }

    std::string toString() const {
        return "Circle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }

    void setCenter(Vector center) override {
        position.x = center.x;
        position.y = center.y;
    }

    void setCenterX(float centerX) override {
        position.x = centerX;
    }

    void setCenterY(float centerY) override {
        position.y = centerY;
    }
};

#endif // SHAPE_HPP