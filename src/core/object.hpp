#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "vector.hpp"

class Object {
public:
    Vector position;
    int width;
    int height;
    Object(int x, int y, int width, int height) :
        position(x, y),
        width(width),
        height(height) {}
    Object(Vector position, int width, int height) :
        position(position),
        width(width),
        height(height) {}
    
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

    virtual void draw() {
        // Placeholder for drawing logic
    }
    
    virtual void update() {
        // Placeholder for update logic
    }

    virtual std::string toString() const {
        return "Object(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    virtual void setCenter(Vector center) {
        // Placeholder for setting the center of the object
    }

    virtual void setCenterX(float centerX) {
        // Placeholder for setting the center X coordinate of the object
    }
    
    virtual void setCenterY(float centerY) {
        // Placeholder for setting the center Y coordinate of the object
    }
};

class MoveableObject{
public:
    Vector velocity;
    Vector acceleration;
    MoveableObject() :
        velocity(0, 0),
        acceleration(0, 0) {}
};

class WithGravity : public MoveableObject {
public:
    WithGravity() : MoveableObject() {}
    void updateGravity() {
        applyGravity();
        applyAcceleration();
    }
    
    void applyGravity() {
        velocity.y += gravity;
    }

    void applyAcceleration() {
        velocity.y += acceleration.y;
    }
};

#endif // OBJECT_HPP