#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "constants.hpp"
#include "shaders.hpp"
#include "object.hpp"
#include "vector.hpp"

void _drawShape(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, GLuint VAO, GLsizei vertexCount, GLuint shader, GLenum primitiveType, GLenum textureUnit, bool useElements, GLuint textureID, vec3 color, bool useColor);
void drawRectangleManual(vec2 centerPosition, float width, float height, float rotation, Texture* texture, GLuint shader, vec3 color=WHITE, bool useColor=NO_USE_COLOR);
void drawCircleManual(vec2 position, float radius, float rotation, Texture* texture, GLuint shader, vec3 color=WHITE, bool useColor=NO_USE_COLOR);

struct Projection {
    float min, max;
};

class Circle : public Object {
public:
    float radius;
    Circle(int x, int y, float radius, float rotation=0.f, Texture* texture=nullptr) :
        Object(x, y, radius * 2, radius * 2, rotation, texture), radius(radius) {}

    Circle(vec2 position, float radius, float rotation=0.f, Texture* texture=nullptr) :
        Object(position, radius * 2, radius * 2, rotation, texture), radius(radius) {}

    // bool isInside(vec2 point) override {
    //     return position.distanceTo(point) <= radius;
    // }

    CollisionInfo getCollisionWith(Circle* other);
    CollisionInfo getCollision(Object& other) override;
 
    virtual void draw() override {
        drawCircleManual(position, radius, rotation, texture, imageShader);
    }

    virtual void update() override {
        ;
    } 

    std::string toString() const {
        return "Circle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }

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

class Rectangle : public Object {
public:
    Rectangle(float x, float y, float width, float height, float rotation=0.f, Texture* texture=nullptr) :
        Object(x, y, width, height, rotation, texture) {}

    Rectangle(vec2 position, float width, float height, float rotation=0.f, Texture* texture=nullptr) :
        Object(position, width, height, rotation, texture) {}

    // bool isInside(vec2 point) override {
    //     return (point.x >= position.x && point.x <= position.x + width &&
    //             point.y >= position.y && point.y <= position.y + height);
    // }

    std::array<vec2, 4> getTransformedVertices();
    Projection projectVertices(std::array<vec2, 4>, vec2& axis);
    CollisionInfo getCollisionWith(Rectangle* other);
    CollisionInfo getCollisionWith(Circle* other);
    CollisionInfo getCollision(Object& other) override;

    virtual void draw() override {
        bool useColor = texture->textureID ? NO_USE_COLOR : USE_COLOR;
        drawRectangleManual(getCenter(), width, height, rotation, texture, imageShader, WHITE, useColor);
    }

    virtual void update() override {
        ;
    }

    std::string toString() const {
        return "Rectangle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

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

#endif // SHAPE_HPP
