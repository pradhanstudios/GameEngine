#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "vector.hpp"
#include "texture.hpp"

class Object {
public:
    Vector position;
    int width;
    int height;
    Texture* texture;
    Object(int x, int y, int width, int height, Texture* texture=nullptr) :
        position(x, y),
        width(width),
        height(height),
	texture(texture) {}
    Object(Vector position, int width, int height, Texture* texture=nullptr) :
        position(position),
        width(width),
        height(height),
	    texture(texture) {}
    
    virtual bool isInside(Vector point) {
        // Placeholder for inside check logic
        return false;
    }

    virtual bool isColliding(Object& other) {
        // Placeholder for collision detection logic
        return false;
    }

    bool isColliding(ObjectList& objects, int i) {
        // placeholder for collision detection with a list of objects
        for (int j = 0; j < objects.size(); j++) {
            if (j != i && isColliding(*objects[j])) {
                return true;
            }
        }
        
        return false;
    } 

    void bindTexture() {
        if (texture)
            glBindTexture(GL_TEXTURE_2D, texture->textureID);
    }

    void unBindTexture() {
        if (texture)
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    virtual void draw() {
        // Placeholder for drawing logic
    }
    
    virtual void update() {
        // Placeholder for update logic
    }

    virtual std::string toString() const {
        return "Object(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    virtual Vector getTopLeft() { return Vector(); }
    virtual float getTopLeftX() { return 0.0f; }
    virtual float getTopLeftY() { return 0.0f; }
    virtual Vector getTopRight() { return Vector(); }
    virtual float getTopRightX() { return 0.0f; }
    virtual float getTopRightY() { return 0.0f; }
    virtual Vector getBottomRight() { return Vector(); }
    virtual float getBottomRightX() { return 0.0f; }
    virtual float getBottomRightY() { return 0.0f; }
    virtual Vector getBottomLeft() { return Vector(); }
    virtual float getBottomLeftX() { return 0.0f; }
    virtual float getBottomLeftY() { return 0.0f; }
    virtual Vector getCenter() { return Vector(); }
    virtual float getCenterX() { return 0.0f; }
    virtual float getCenterY() { return 0.0f; }
    virtual void setCenter(Vector center) {}
    virtual void setCenterX(float centerX) {}
    virtual void setCenterY(float centerY) {}
    virtual void setTopLeft(Vector topLeft) {}
    virtual void setTopLeftX(float topLeftX) {}
    virtual void setTopLeftY(float topLeftY) {}
    virtual void setBottomLeft(Vector bottomRight) {}
    virtual void setBottomLeftX(float bottomRightX) {}
    virtual void setBottomLeftY(float bottomRightY) {}
    virtual void setBottomRight(Vector bottomRight) {}
    virtual void setBottomRightX(float bottomRightX) {}
    virtual void setBottomRightY(float bottomRightY) {}
    virtual void setTopRight(Vector topRight) {}
    virtual void setTopRightX(float topRightX) {}
    virtual void setTopRightY(float topRightY) {}
};

class Moveable {
public:
    Vector velocity;
    Vector acceleration;
    Moveable() :
        velocity(0, 0),
        acceleration(0, 0) {}
};

class WithGravity : public Moveable {
public:
    WithGravity() : Moveable() {}
    void updateGravity() {
        applyGravity();
        applyAcceleration();
    }
    
    void applyGravity() {
        acceleration.y = gravity;
    }

    void applyAcceleration() {
        velocity.y += acceleration.y * 0.5f;
    }
};

class Collision {
public:
    uint8_t collisions;    
    Collision() : collisions(0) {}
    void setCollisionDirection(Object &object, Object& other) {
        collisions = 0;
        Vector position = object.position;
        if (position.x < other.position.x) {
            collisions |= LEFT_COLLISION;
            object.setTopLeftX(other.getTopRightX() + epsilon);
        }
        if (position.x > other.position.x + other.width) {
            collisions |= RIGHT_COLLISION;
            object.setTopRightX(other.getTopLeftX() - epsilon);
        }
        if (position.y > other.position.y) {
            collisions |= TOP_COLLISION;
            object.setTopRightY(other.getBottomLeftY() + epsilon);
        }
        if (position.y < other.position.y + other.height) {
            collisions |= BOTTOM_COLLISION;
            object.setBottomLeftY(other.getTopLeftY() - epsilon);
        }
    }

    bool colliding(uint8_t direction) {
        return collisions & direction;
    }

    virtual void applyCollisions(Vector& velocity, Vector& acceleration) {
        if (colliding(LEFT_COLLISION) && velocity.x < 0) {
            velocity.x = 0;
            acceleration.x = 0;
        }

        if (colliding(RIGHT_COLLISION) && velocity.x > 0) {
            velocity.x = 0;
            acceleration.x = 0;
        }

        if (colliding(TOP_COLLISION) && velocity.y < 0) {
            velocity.y = 0;
            acceleration.y = 0;
        }

        if (colliding(BOTTOM_COLLISION) && velocity.y > 0) {
            velocity.y = 0;
            acceleration.y = 0;
        }
    }
};

#endif // OBJECT_HPP
