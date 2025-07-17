#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "constants.hpp"
#include "shaders.hpp"
#include "object.hpp"
#include "collision.hpp"
#include "vector.hpp"
#include "draw.hpp"

class Rectangle : public Object {
public:
    Rectangle(float x, float y, float width, float height, float rotation=0.f, Texture* texture=nullptr) :
        Object(x, y, width, height, rotation, texture) {}

    Rectangle(vec2 position, float width, float height, float rotation=0.f, Texture* texture=nullptr) :
        Object(position, width, height, rotation, texture) {}

    std::string toString() const {
        return "Rectangle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    virtual void update() override {}

    virtual void draw() override {
        drawRectangle(this);
    }

    CollisionInfo collision(Object* other) override;

    vec2 getCenter() override {
        return vec2(position.x + width / 2, position.y + height / 2);
    }

    float getCenterX() override {
        return position.x + width / 2;
    }

    float getCenterY() override {
        return position.y + height / 2;
    }

    void setCenter(vec2 center) override {
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
    Circle(int x, int y, float radius, float rotation=0.f, Texture* texture=nullptr) :
        Object(x, y, radius * 2, radius * 2, rotation, texture), radius(radius) {}

    Circle(vec2 position, float radius, float rotation=0.f, Texture* texture=nullptr) :
        Object(position, radius * 2, radius * 2, rotation, texture), radius(radius) {}

    virtual void update() override {}

    virtual void draw() override {
        drawCircle(this);
    }

    CollisionInfo collision(Object* other) override;

    vec2 getCenter() override {
        return position;
    }

    float getCenterX() override {
        return position.x;
    }

    float getCenterY() override {
        return position.y;
    }

    void setCenter(vec2 center) override {
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
