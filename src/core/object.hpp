#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "shaders.hpp"
#include "vector.hpp"
#include "texture.hpp"
#include "collision.hpp"

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

    virtual std::string toString() const {
        return "Object(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    virtual void update() {}
    virtual void draw() {}
    virtual CollisionInfo collision(Object*) { return CollisionInfo(); }
    virtual vec2 getCenter() { return vec2(); }
    virtual float getCenterX() { return 0.0f; }
    virtual float getCenterY() { return 0.0f; }
    virtual void setCenter(vec2) {}
    virtual void setCenterX(float) {}
    virtual void setCenterY(float) {}
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
    void applyGravity() {
        acceleration.y = Physics::Gravity;
    }
};


#endif // OBJECT_HPP
