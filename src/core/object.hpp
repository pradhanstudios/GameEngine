#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "shaders.hpp"
#include "vector.hpp"
#include "texture.hpp"

struct CollisionInfo {
    vec2 normal;
    vec2 mtv;
    bool collision;
    CollisionInfo() : normal(vec2(0.f, 0.f)), mtv(vec2(0.f, 0.f)) {}
};

class Object {
public:
    vec2 position;
    float width;
    float height;
    float rotation;
    Texture* texture;
    Object(float x, float y, float width, float height, float rotation=0.f, Texture* texture=nullptr) :
        position(x, y),
        width(width),
        height(height),
        rotation(rotation),
	    texture(texture) {}
    Object(vec2 position, float width, float height, float rotation=0.f, Texture* texture=nullptr) :
        position(position),
        width(width),
        height(height),
        rotation(rotation),
	    texture(texture) {}
    
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

    virtual vec2 getCenter() { return vec2(); }
    virtual float getCenterX() { return 0.0f; }
    virtual float getCenterY() { return 0.0f; }
    virtual void setCenter(vec2) {}
    virtual void setCenterX(float) {}
    virtual void setCenterY(float) {}
};

class Collision {
public:
    CollisionInfo info;

    Collision() : info(CollisionInfo()) {}
    void setCollision(Object& object, Object& other) {
        info = object.getCollision(other);
    }

    virtual void applyCollisions(vec2& position, vec2& velocity, vec2& acceleration) {
        if (!info.collision) {
            return;
        } 
        
        position += info.mtv;
        float velocityDotNormal = glm::dot(velocity, info.normal);

        if (velocityDotNormal < 0.f) {
            vec2 normalVelocityComponent = info.normal * velocityDotNormal;
            vec2 tangentialVelocityComponent = velocity - normalVelocityComponent;
            const float bounce = 0.f;
            normalVelocityComponent = -normalVelocityComponent * bounce; 
            tangentialVelocityComponent *= (1.f - friction);
            velocity = normalVelocityComponent + tangentialVelocityComponent;
        }

        // float accelerationDotNormal = glm::dot(acceleration, info.normal);

        // Later implement more advanced accleration system
        if (glm::dot(info.normal, vec2(0.0f, 1.0f)) > 0.7f) { // Normal is pointing mostly upwards (like a floor)
            if (acceleration.y < 0) { 
                acceleration.y = 0; 
            }

            // If the object's vertical velocity is very small after collision,
            if (glm::abs(velocity.y) < 0.1f) { // Threshold for "stopped vertically"
                velocity.y = 0;
            }
        }

        if (glm::dot(info.normal, vec2(0.0f, -1.0f)) > 0.7f) {
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
    vec2 velocity;
    vec2 acceleration;
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
