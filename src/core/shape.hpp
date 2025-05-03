#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "constants.hpp"
#include "object.hpp"
#include "vector.hpp"

class Rectangle : public Object {
public:
    Rectangle(int x, int y, int width, int height, Texture* texture=nullptr) :
        Object(x, y, width, height, texture) {}

    Rectangle(Vector position, int width, int height, Texture* texture=nullptr) :
        Object(position, width, height, texture) {}

    bool isInside(Vector point) override {
        return (point.x >= position.x && point.x <= position.x + width &&
                point.y >= position.y && point.y <= position.y + height);
    }

    bool isColliding(Object& other) override;

    void draw() override {
        initDraw();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(position.x, position.y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(position.x + width, position.y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(position.x + width, position.y + height);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(position.x, position.y + height);
        glEnd();
        unInitDraw();
    }

    virtual void update() override {
        ;
    }

    std::string toString() const {
        return "Rectangle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    Vector getTopLeft() override {
        return position;
    }

    float getTopLeftX() override {
        return position.x;
    }

    float getTopLeftY() override {
        return position.y;
    }

    Vector getBottomRight() override {
        return Vector(position.x + width, position.y + height);
    }

    float getBottomRightX() override {
        return position.x + width;
    }

    float getBottomRightY() override {
        return position.y + height;
    }

    Vector getBottomLeft() override {
        return Vector(position.x, position.y + height);
    }

    float getBottomLeftX() override {
        return position.x;
    }

    float getBottomLeftY() override {
        return position.y + height;
    }

    Vector getTopRight() override {
        return Vector(position.x + width, position.y);
    }

    float getTopRightX() override {
        return position.x + width;
    }

    float getTopRightY() override {
        return position.y;
    }

    Vector getCenter() override {
        return Vector(position.x + width / 2, position.y + height / 2);
    }

    float getCenterX() override {
        return position.x + width / 2;
    }

    float getCenterY() override {
        return position.y + height / 2;
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

    void setTopLeft(Vector topLeft) override {
        position.x = topLeft.x;
        position.y = topLeft.y;
    }

    void setTopLeftX(float topLeftX) override {
        position.x = topLeftX;
    }

    void setTopLeftY(float topLeftY) override {
        position.y = topLeftY;
    }

    void setBottomLeft(Vector bottomLeft) override {
        position.x = bottomLeft.x;
        position.y = bottomLeft.y + height;
    }

    void setBottomLeftX(float bottomLeftX) override {
        position.x = bottomLeftX;
    }

    void setBottomLeftY(float bottomLeftY) override {
        position.y = bottomLeftY + height;
    }

    void setBottomRight(Vector bottomRight) override {
        position.x = bottomRight.x + width;
        position.y = bottomRight.y + height;
    }

    void setBottomRightX(float bottomRightX) override {
        position.x = bottomRightX + width;
    }

    void setBottomRightY(float bottomRightY) override {
        position.y = bottomRightY + height;
    }

    void setTopRight(Vector topRight) override {
        position.x = topRight.x + width;
        position.y = topRight.y;
    }

    void setTopRightX(float topRightX) override {
        position.x = topRightX + width;
    }

    void setTopRightY(float topRightY) override {
        position.y = topRightY;
    }
};

class Circle : public Object {
public:
    float radius;
    Circle(int x, int y, float radius, Texture* texture=nullptr) :
        Object(x, y, radius * 2, radius * 2, texture), radius(radius) {}

    Circle(Vector position, float radius, Texture* texture=nullptr) :
        Object(position, radius * 2, radius * 2, texture), radius(radius) {}

    bool isInside(Vector point) override {
        return position.distanceTo(point) <= radius;
    }

    bool isColliding(Object& other) override;

    void draw() override {
        initDraw();
        // glBegin(GL_TRIANGLE_FAN);
        //     glTexCoord2f(0.5f, 0.5f);
        //     glVertex2f(position.x, position.y);
        //     for (int i = 0; i <= TRIANGLE_COUNT_CIRCLE; i++) {
        //         float angle = i * tau / TRIANGLE_COUNT_CIRCLE;
        //         float x = radius * cos(angle);
        //         float y = radius * sin(angle);
        //         float texScale = 0.1f;
        //         glTexCoord2f(0.5f + x * texScale, 0.5f + y * texScale);
        //         glVertex2f(position.x + radius * x, position.y + radius * y);
        //     }
        // glEnd();
        // unBindTexture();
        // glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(position.x - radius, position.y - radius);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(position.x + radius, position.y - radius);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(position.x + radius, position.y + radius);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(position.x - radius, position.y + radius);
        glEnd();
        unInitDraw();
    }

    virtual void update() override {
        ;
    } 

    std::string toString() const {
        return "Circle(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(radius) + ")";
    }

    Vector getTopLeft() override {
        return Vector(position.x - radius, position.y - radius);
    }

    float getTopLeftX() override {
        return position.x - radius;
    }

    float getTopLeftY() override {
        return position.y - radius;
    }

    Vector getBottomRight() override {
        return Vector(position.x + radius, position.y + radius);
    }

    float getBottomRightX() override {
        return position.x + radius;
    }

    float getBottomRightY() override {
        return position.y + radius;
    }

    Vector getBottomLeft() override {
        return Vector(position.x - radius, position.y + radius);
    }

    float getBottomLeftX() override {
        return position.x - radius;
    }

    float getBottomLeftY() override {
        return position.y + radius;
    }

    Vector getTopRight() override {
        return Vector(position.x + radius, position.y - radius);
    }

    float getTopRightX() override {
        return position.x + radius;
    }

    float getTopRightY() override {
        return position.y - radius;
    }

    Vector getCenter() override {
        return position;
    }

    float getCenterX() override {
        return position.x;
    }

    float getCenterY() override {
        return position.y;
    }

    void setTopLeft(Vector topLeft) override {
        position.x = topLeft.x + radius;
        position.y = topLeft.y + radius;
    }

    void setTopLeftX(float topLeftX) override {
        position.x = topLeftX + radius;
    }

    void setTopLeftY(float topLeftY) override {
        position.y = topLeftY + radius;
    }

    void setBottomLeft(Vector bottomLeft) override {
        position.x = bottomLeft.x + radius;
        position.y = bottomLeft.y - radius;
    }

    void setBottomLeftX(float bottomLeftX) override {
        position.x = bottomLeftX + radius;
    }

    void setBottomLeftY(float bottomLeftY) override {
        position.y = bottomLeftY - radius;
    }

    void setBottomRight(Vector bottomRight) override {
        position.x = bottomRight.x - radius;
        position.y = bottomRight.y - radius;
    }

    void setBottomRightX(float bottomRightX) override {
        position.x = bottomRightX - radius;
    }

    void setBottomRightY(float bottomRightY) override {
        position.y = bottomRightY - radius;
    }

    void setTopRight(Vector topRight) override {
        position.x = topRight.x - radius;
        position.y = topRight.y + radius;
    }

    void setTopRightX(float topRightX) override {
        position.x = topRightX - radius;
    }

    void setTopRightY(float topRightY) override {
        position.y = topRightY + radius;
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
