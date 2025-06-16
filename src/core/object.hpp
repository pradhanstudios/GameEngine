#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "shaders.hpp"
#include "vector.hpp"
#include "texture.hpp"

struct CollisionInfo {
    glm::vec2 normal;
    glm::vec2 mtv;
    bool collision;
    CollisionInfo() : normal(glm::vec2(0.f, 0.f)), mtv(glm::vec2(0.f, 0.f)) {}
};

class Object {
public:
    Vector position;
    int width;
    int height;
    Texture* texture;
    float rotation;
    Object(int x, int y, int width, int height, Texture* texture=nullptr, float rotation=0.f) :
        position(x, y),
        width(width),
        height(height),
	    texture(texture),
        rotation(rotation) {}
    Object(Vector position, int width, int height, Texture* texture=nullptr, float rotation=0.f) :
        position(position),
        width(width),
        height(height),
	    texture(texture),
        rotation(rotation) {}
    
    virtual bool isInside(Vector) {
        // Placeholder for inside check logic
        return false;
    }

    virtual CollisionInfo getCollision(Object&) {
        // Placeholder for collision detection logic
        return CollisionInfo();
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
    virtual void setCenter(Vector) {}
    virtual void setCenterX(float) {}
    virtual void setCenterY(float) {}
    virtual void setTopLeft(Vector) {}
    virtual void setTopLeftX(float) {}
    virtual void setTopLeftY(float) {}
    virtual void setBottomLeft(Vector) {}
    virtual void setBottomLeftX(float) {}
    virtual void setBottomLeftY(float) {}
    virtual void setBottomRight(Vector) {}
    virtual void setBottomRightX(float) {}
    virtual void setBottomRightY(float) {}
    virtual void setTopRight(Vector) {}
    virtual void setTopRightX(float) {}
    virtual void setTopRightY(float) {}
};

class Collision {
public:
    CollisionInfo info;

    Collision() : info(CollisionInfo()) {}
    // void setCollisionDirection(Object &object, Object& other) {
    //     collisions = 0;
    //     Vector position = object.position;
    //     if (position.x < other.position.x) {
    //         collisions |= LEFT_COLLISION;
    //     }
    //
    //     if (position.x > other.position.x + other.width) {
    //         collisions |= RIGHT_COLLISION;
    //     }
    //
    //     if (position.y > other.position.y) {
    //         collisions |= TOP_COLLISION;
    //     }
    //
    //     if (position.y < other.position.y + other.height) {
    //         collisions |= BOTTOM_COLLISION;
    //     }
    // }
    //
    void setCollision(Object& object, Object& other) {
        info = object.getCollision(other);
    }

    virtual void applyCollisions(Vector& position, Vector& velocity, Vector& acceleration) {
        if (!info.collision) {
            return;
        } 
        
        position.x += info.mtv.x;
        position.y += info.mtv.y;

        glm::vec2 velocity_glm = glm::vec2(velocity.x, velocity.y);
        float velocityDotNormal = glm::dot(velocity_glm, info.normal);

        if (velocityDotNormal < 0.f) {
            glm::vec2 normalVelocityComponent = info.normal * velocityDotNormal;
            glm::vec2 tangentialVelocityComponent = velocity_glm - normalVelocityComponent;
            const float bounce = 0.f;
            normalVelocityComponent = -normalVelocityComponent * bounce; 
            tangentialVelocityComponent *= (1.f - friction);
            glm::vec2 resolvedVelocity = normalVelocityComponent + tangentialVelocityComponent;
            velocity.x = resolvedVelocity.x;
            velocity.y = resolvedVelocity.y;
        }

        glm::vec2 acceleration_glm = glm::vec2(acceleration.x, acceleration.y);
        float accelerationDotNormal = glm::dot(acceleration_glm, info.normal);

        // Later implement more advanced accleration system, for now it will just 
        if (glm::dot(info.normal, glm::vec2(0.0f, 1.0f)) > 0.7f) { // Normal is pointing mostly upwards (like a floor)
            if (acceleration.y < 0) { 
                acceleration.y = 0; 
            }

            // If the object's vertical velocity is very small after collision,
            // set it to zero to prevent tiny bounces/jittering on a flat surface.
            if (glm::abs(velocity.y) < 0.1f) { // Threshold for "stopped vertically"
                velocity.y = 0;
            }
        }

        if (glm::dot(info.normal, glm::vec2(0.0f, -1.0f)) > 0.7f) {
            if (acceleration.y > 0) { // If acceleration is pulling upwards (e.g. anti-gravity)
                acceleration.y = 0;
            }
            if (glm::abs(velocity.y) < 0.1f) {
                velocity.y = 0;
            }
        }
    }
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


#endif // OBJECT_HPP
