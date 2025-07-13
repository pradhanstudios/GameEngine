#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "constants.hpp"
#include "vector.hpp"
#include "object.hpp"
#include "shape.hpp"

struct CollisionInfo {
    vec2 normal;
    vec2 mtv;
    bool collision;
    CollisionInfo() : normal(vec00), mtv(vec00), collision(false) {}

    CollisionInfo operator+(const CollisionInfo& other) {
        CollisionInfo res = CollisionInfo();
        res.normal = normal + other.normal;
        res.mtv = mtv + other.mtv;
        res.collision = collision || other.collision;
        
        return res;
    }

    CollisionInfo& operator+=(const CollisionInfo& other) {
        *this = *this + other;
        return *this;
    }
};

CollisionInfo colliding(Rectangle* rect1, Rectangle* rect2);
CollisionInfo colliding(Rectangle* rect, Circle* circle);
CollisionInfo colliding(Circle* circle1, Circle* circle2);
CollisionInfo colliding(Circle* circle, Rectangle* rect);

class Collision {
public:
    CollisionInfo info;

    Collision() : info(CollisionInfo()) {}

    inline void resetCollision() {
        info = CollisionInfo();    
    }

    inline void setCollision(Object& object, Object& other) {
        info += object.getCollision(other);
    }

    virtual void applyCollisions(vec2& position, vec2& velocity, vec2& acceleration);
};

#endif // COLLISION_HPP
